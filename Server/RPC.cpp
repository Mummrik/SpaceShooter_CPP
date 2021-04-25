#include "RPC.h"
#include "Connection.h"
#include "Server.h"
#include "Player.h"

void RPC::Invoke(RPC& rpc, const PacketType& type, Connection* client, NetworkMessage& data)
{
	if ((uint16_t)type >= 0 && type < PacketType::MAX_LENGTH && m_Rpc[(size_t)type] != nullptr)
	{
		//std::lock_guard<std::mutex> lock(m_Server->mutex);
		std::invoke(m_Rpc[(size_t)type], rpc, client, data);
	}
}

void RPC::Init()
{
	//Register RPC's here
	m_Rpc[(size_t)PacketType::Disconnect] = &RPC::Disconnect;
	m_Rpc[(size_t)PacketType::HandShake] = &RPC::HandShake;
	m_Rpc[(size_t)PacketType::Movement] = &RPC::Movement;
	m_Rpc[(size_t)PacketType::RotatePlayer] = &RPC::Rotation;
	m_Rpc[(size_t)PacketType::FireBullet] = &RPC::FireBullet;
}

void RPC::Disconnect(Connection* client, NetworkMessage& data)
{
	std::cout << ">> " << client->RemoteEndpoint << " => RPC::Disconnect()" << std::endl;
	if (auto player = m_Server->GetPlayer(client->Id))
		player->IsActive = false;

	m_Server->TerminateClient(client);
}

void RPC::HandShake(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> " << client->RemoteEndpoint << " => RPC::HandShake()" << std::endl;
	client->Authorized = true;

	//Send existing players to new connection
	for (auto& c : m_Server->GetConnections())
	{
		if (auto player = m_Server->GetPlayer(c->Id))
		{
			NetworkMessage msg(PacketType::NewPlayer);
			msg.Write(player->GetPlayerUid());
			msg.Write(player->GetPlayerSpriteId());
			msg.Write(player->GetPlayerPosition().x);
			msg.Write(player->GetPlayerPosition().y);
			msg.Write(player->GetPlayerRotation());
			client->Send(msg, true);
		}
	}

	std::lock_guard<std::mutex> lock(m_Server->mutex);
	m_Server->SpawnPlayer.push(client->Id);
}

void RPC::Movement(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> " << client->RemoteEndpoint << " => RPC::Movement()" << std::endl;
	if (auto player = m_Server->GetPlayer(client->Id))
	{
		player->AddVelocity(Vec2d(data.ReadFloat(), data.ReadFloat()));

		NetworkMessage msg(PacketType::AnimateJetEngine);
		msg.Write(client->Id);
		m_Server->SendToAll(msg, true);
	}
}

void RPC::Rotation(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> " << client->RemoteEndpoint << " => RPC::Rotation()" << std::endl;
	NetworkMessage msg(PacketType::RotatePlayer);
	msg.Write(client->Id);
	msg.Write(data.ReadFloat());
	m_Server->SendToAll(msg, false);
}

void RPC::FireBullet(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> " << client->RemoteEndpoint << " => RPC::FireBullet()" << std::endl;
	if (auto player = m_Server->GetPlayer(client->Id))
	{
		if (player->CanShoot())
		{
			player->SetCooldown(1.0f);
			Vec2d velocity(data.ReadFloat(), data.ReadFloat());
			m_Server->NewBullet(player->GetPlayerPosition() + (velocity * 10), velocity);
		}
	}
}
