// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Server.h"
#include "Player.h"
#include "Bullet.h"


void Server::Start(uint16_t port)
{
	std::cout << "Server starting on port " << port << std::endl;
	m_Listener = std::thread(&Server::OnListen, this);
}

bool Server::Shutdown()
{
	IsRunning = false;

	if (m_Listener.joinable())
		m_Listener.join();

	if (m_Game.joinable())
		m_Game.join();

	if (m_Socket.is_open())
		m_Socket.close();

	return true;
}

void TestNetworkMessage()
{
	NetworkMessage testMsg(PacketType::MAX_LENGTH);
	testMsg.Write((int8_t)INT8_MIN);
	testMsg.Write((int16_t)INT16_MIN);
	testMsg.Write(INT32_MIN);
	testMsg.Write(INT64_MIN);

	testMsg.Write(UINT8_MAX);
	testMsg.Write(UINT16_MAX);
	testMsg.Write(UINT32_MAX);
	testMsg.Write(UINT64_MAX);
	testMsg.Write(110.23156757f);
	testMsg.PrepareSend();

	NetworkMessage data(testMsg.Body);
	std::cout << "\n-- Test Message --\nMessageType: " << (uint16_t)data.GetPacketType() << std::endl;
	int8_t test_int8_t = data.ReadInt8();
	std::cout << "\nsigned\nint8_t " << (int16_t)test_int8_t << std::endl;

	int16_t test_int16_t = data.ReadInt16();
	std::cout << "int16_t " << (int16_t)test_int16_t << std::endl;

	int32_t test_int32_t = data.ReadInt32();
	std::cout << "int32_t " << (int32_t)test_int32_t << std::endl;

	int64_t test_int64_t = data.ReadInt64();
	std::cout << "int64_t " << (int64_t)test_int64_t << std::endl;

	uint8_t test_uint8_t = data.ReadUint8();
	std::cout << "\nunsigned\nuint8_t " << (uint16_t)test_uint8_t << std::endl;

	uint16_t test_uint16_t = data.ReadUint16();
	std::cout << "uint16_t " << (uint16_t)test_uint16_t << std::endl;

	uint32_t test_uint32_t = data.ReadUint32();
	std::cout << "uint32_t " << (uint32_t)test_uint32_t << std::endl;

	uint64_t test_uint64_t = data.ReadUint64();
	std::cout << "uint64_t " << (uint64_t)test_uint64_t << std::endl;

	float test_float = data.ReadFloat();
	std::cout << "float " << (float)test_float << std::endl;
}

void Server::OnListen()
{
	IsRunning = true;
	std::cout << "Listener thread started" << std::endl;
	asio::ip::udp::endpoint RemoteEndpoint;
	std::array<char, 1024> InBuffer;
	size_t recvSize = 0;

	m_Game = std::thread(&Server::GameLoop, this);

	// Testing NetworkMessage 
	//TestNetworkMessage();

	while (IsRunning)
	{
		recvSize = m_Socket.receive_from(asio::buffer(InBuffer), RemoteEndpoint);

		if (recvSize > 0)
		{
			std::vector<char> data(InBuffer.begin(), InBuffer.begin() + recvSize);
			OnHandle(data, RemoteEndpoint);
		}
	}

	std::cout << "Listener thread terminated" << std::endl;
}

void Server::GameLoop()
{
	std::cout << "GameLoop thread started" << std::endl;
	float deltaTime;
	auto currentTime = std::chrono::system_clock::now();

	while (IsRunning)
	{
		// Calculate deltatime
		auto newTime = std::chrono::system_clock::now();
		deltaTime = (newTime - currentTime).count() * 0.0000001f;
		currentTime = newTime;

		RandomSpriteId = rand() % 10;

		for (auto player : m_Players)
		{
			if (player == nullptr)
				break;

			player->OnUpdate(this, deltaTime);
		}

		for (auto bullet : m_Bullets)
		{
			if (bullet == nullptr)
				break;

			bullet->OnUpdate(this, deltaTime);
		}

		CheckRemovePlayer();
		SpawnNewPlayer();
		CheckRemoveBullet();
		SpawnNewBullet();
	}

	std::cout << "GameLoop thread terminated" << std::endl;
}

uint16_t Server::FindEmptyPlayerIndex()
{
	auto length = m_Players.size();
	for (uint16_t i = 0; i < length; i++)
		if (m_Players[i] == nullptr)
			return i;

	return -1;
}

uint16_t Server::FindEmptyBulletIndex()
{
	auto length = m_Bullets.size();
	for (uint16_t i = 0; i < length; i++)
		if (m_Bullets[i] == nullptr)
			return i;

	return -1;
}

void Server::CheckRemovePlayer()
{
	if (size_t length = m_RemovePlayer.size() > 0)
	{
		for (size_t i = 0; i < length; i++)
		{
			uint64_t uid = m_RemovePlayer.front();
			m_RemovePlayer.pop();

			size_t removeIndex = GetPlayerIndex(uid);
			auto lastElement = GetLastPlayerElement();
			size_t lastElementIndex = GetPlayerIndex(lastElement->GetPlayerUid());

			if (removeIndex == lastElementIndex)
			{
				m_Players[removeIndex] = nullptr;
			}
			else
			{
				m_Players[removeIndex] = lastElement;
				m_Players[lastElementIndex] = nullptr;
			}

			NetworkMessage msg(PacketType::RemovePlayer);
			msg.Write(uid);
			SendToAll(msg, true);
		}
	}
}

void Server::SpawnNewPlayer()
{
	if (size_t length = SpawnPlayer.size() > 0)
	{
		std::lock_guard<std::mutex> lock(mutex);
		for (size_t i = 0; i < length; i++)
		{
			uint64_t uid = SpawnPlayer.front();
			SpawnPlayer.pop();

			uint16_t freeIndex = FindEmptyPlayerIndex();
			if (freeIndex > m_Players.size())
			{
				std::cout << "Warning: Couldn't add Player, maximum amount of players reached." << std::endl;
				return;
			}

			auto player = std::make_shared<Player>(uid, RandomSpriteId);
			m_Players[freeIndex] = player;

			player->IsActive = true;

			NetworkMessage msg(PacketType::NewPlayer);
			msg.Write(player->GetPlayerUid());
			msg.Write(player->GetPlayerSpriteId());
			msg.Write(player->GetPlayerPosition().x);
			msg.Write(player->GetPlayerPosition().y);
			msg.Write(player->GetPlayerRotation());
			SendToAll(msg, true);
		}
	}
}

void Server::SpawnNewBullet()
{
	if (size_t length = SpawnBullet.size() > 0)
	{
		std::lock_guard<std::mutex> lock(mutex);
		for (size_t i = 0; i < length; i++)
		{
			auto bullet = SpawnBullet.front();
			SpawnBullet.pop();

			uint16_t freeIndex = FindEmptyBulletIndex();
			if (freeIndex > m_Bullets.size())
			{
				std::cout << "Warning: Couldn't add bullet, maximum amount of bullets reached." << std::endl;
				return;
			}

			m_Bullets[freeIndex] = bullet;
			bullet->IsActive = true;

			NetworkMessage msg(PacketType::FireBullet);
			msg.Write(bullet->GetBulletUid());
			msg.Write(bullet->GetPosition().x);
			msg.Write(bullet->GetPosition().y);
			msg.Write(bullet->GetRotation());
			SendToAll(msg, true);
		}
	}
}

void Server::CheckRemoveBullet()
{
	if (size_t length = RemoveBullet.size() > 0)
	{
		for (size_t i = 0; i < length; i++)
		{
			uint64_t uid = RemoveBullet.front();
			RemoveBullet.pop();

			size_t removeIndex = GetBulletIndex(uid);
			auto lastElement = GetLastBulletElement();
			size_t lastElementIndex = GetBulletIndex(lastElement->GetBulletUid());

			if (removeIndex == lastElementIndex)
			{
				m_Bullets[removeIndex] = nullptr;
			}
			else
			{
				m_Bullets[removeIndex] = lastElement;
				m_Bullets[lastElementIndex] = nullptr;
			}

			NetworkMessage msg(PacketType::RemoveBullet);
			msg.Write(uid);
			SendToAll(msg, true);
		}
	}
}

void Server::OnHandle(const std::vector<char>& Data, const asio::ip::udp::endpoint& RemoteEndpoint)
{
	Connection* client;
	if (client = GetConnection(RemoteEndpoint))
	{
		NetworkMessage msg(Data);
		if (client->Authorized)
		{
			m_Rpc.Invoke(m_Rpc, msg.GetPacketType(), client, msg);
		}
		else if (!client->Authorized && msg.GetPacketType() == PacketType::HandShake)
		{
			m_Rpc.Invoke(m_Rpc, msg.GetPacketType(), client, msg);
		}
		else
		{
			TerminateClient(client);
		}
	}
	else
	{
		client = new Connection(&m_Socket, RemoteEndpoint);
		m_Connections.push_back(client);

		std::cout << "New Connection | " << client->RemoteEndpoint << std::endl;

		NetworkMessage msg(PacketType::HandShake);
		msg.Write(client->Id);
		client->Send(msg, true);
	}
}

void Server::SendToAll(NetworkMessage& msg, bool reliable)
{
	msg.PrepareSend();
	for (auto& client : m_Connections)
	{
		client->Socket->send_to(asio::buffer(msg.Body), client->RemoteEndpoint);
		/*client->Socket->async_send_to(
			asio::buffer(msg.Body), client->RemoteEndpoint,
			[&](std::error_code ec, std::size_t length)
			{
				if (ec)
				{
					std::cout << "Sent " << length << " bytes" << std::endl;
				}
			});*/
	}
}

Connection* Server::GetConnection(const asio::ip::udp::endpoint& RemoteEndpoint)
{
	auto it = std::find_if(
		m_Connections.begin(), m_Connections.end(),
		[RemoteEndpoint](auto other) { return RemoteEndpoint == other->RemoteEndpoint; });

	if (it != m_Connections.end())
	{
		return *it;
	}

	return nullptr;
}

Connection* Server::GetConnection(uint64_t uid)
{
	auto it = std::find_if(
		m_Connections.begin(), m_Connections.end(),
		[uid](auto other) { return uid == other->Id; });

	if (it != m_Connections.end())
	{
		return *it;
	}

	return nullptr;
}

std::vector<Connection*> Server::GetConnections()
{
	return m_Connections;
}

void Server::TerminateClient(Connection* client)
{
	uint64_t uid = client->Id;
	auto it = std::find_if(m_Connections.begin(), m_Connections.end(),
		[&client](auto other) { return client->RemoteEndpoint == other->RemoteEndpoint; });

	if (it != m_Connections.end())
	{
		NetworkMessage msg(PacketType::Disconnect);
		client->Send(msg, true);
		delete* it;
		m_Connections.erase(it);
	}

	m_RemovePlayer.push(uid);
}

void Server::NewBullet(uint64_t ownerId, Vec2d position, Vec2d velocity, float rotation)
{
	std::lock_guard<std::mutex> lock(mutex);
	auto bullet = std::make_shared<Bullet>(ownerId, position, velocity, rotation);
	SpawnBullet.emplace(bullet);
}

size_t Server::GetPlayerIndex(uint64_t uid)
{
	size_t length = m_Players.size();
	for (size_t i = 0; i < length; i++)
	{
		auto player = m_Players[i];
		if (player->GetPlayerUid() == uid)
		{
			return i;
		}
	}
	return -1;
}

std::shared_ptr<Player> Server::GetLastPlayerElement()
{
	std::shared_ptr<Player> player = nullptr;
	size_t length = m_Players.size();
	for (size_t i = 0; i < length; i++)
	{
		if (m_Players[i] == nullptr)
			break;

		player = m_Players[i];
	}

	return player;
}

std::shared_ptr<Player> Server::GetPlayer(uint64_t uid)
{
	size_t length = m_Players.size();
	for (size_t i = 0; i < length; i++)
	{
		if (m_Players[i] == nullptr)
			return nullptr;

		if (m_Players[i]->GetPlayerUid() == uid)
			return m_Players[i];
	}

	return nullptr;
}

size_t Server::GetBulletIndex(uint64_t uid)
{
	size_t length = m_Bullets.size();
	for (size_t i = 0; i < length; i++)
	{
		auto bullet = m_Bullets[i];
		if (bullet->GetBulletUid() == uid)
		{
			return i;
		}
	}
	return -1;
}

std::shared_ptr<Bullet> Server::GetLastBulletElement()
{
	std::shared_ptr<Bullet> bullet = nullptr;
	size_t length = m_Bullets.size();
	for (size_t i = 0; i < length; i++)
	{
		if (m_Bullets[i] == nullptr)
			break;

		bullet = m_Bullets[i];
	}

	return bullet;
}

std::array<std::shared_ptr<Player>, 1000>& Server::GetPlayers()
{
	return m_Players;
}
