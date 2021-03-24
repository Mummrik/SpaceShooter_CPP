// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Server.h"

bool Server::Start()
{
	if (IsRunning)
		return false;

	m_Listener = std::thread(&Server::OnListen, this);
	return true;
}

bool Server::Shutdown()
{
	IsRunning = false;

	if (m_Listener.joinable())
		m_Listener.join();

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
}

void Server::OnListen()
{
	std::cout << "Listener thread started" << std::endl;
	asio::ip::udp::endpoint RemoteEndpoint;
	std::array<char, 1024> InBuffer;
	size_t recvSize = 0;
	IsRunning = true;

	// Testing NetworkMessage 
	//TestNetworkMessage();

	while (IsRunning)
	{
		try
		{
			recvSize = m_Socket.receive_from(asio::buffer(InBuffer), RemoteEndpoint);
		}
		catch (const std::exception& ex)
		{
			std::cout << "\nException: " << ex.what() << "\n" << std::endl;
		}

		if (recvSize > 0)
		{
			std::vector<char> data(InBuffer.begin(), InBuffer.begin() + recvSize);
			OnHandle(data, RemoteEndpoint);
		}
	}

	std::cout << "Listener thread terminated" << std::endl;
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
		Connection newClient(&m_Socket, RemoteEndpoint);
		m_Connections.push_back(newClient);

		client = &m_Connections[m_Connections.size() - 1];
		if (client->SetId())
		{
			std::cout << "New Connection | " << client->RemoteEndpoint << std::endl;

			NetworkMessage msg(PacketType::HandShake);
			msg.Write(client->Id);
			client->Send(msg);
		}
		else
		{
			TerminateClient(client);
		}
	}
}

Connection* Server::GetConnection(const asio::ip::udp::endpoint& RemoteEndpoint)
{
	size_t size = m_Connections.size();
	for (size_t i = 0; i < size; i++)
	{
		if (m_Connections[i].RemoteEndpoint == RemoteEndpoint)
		{
			return &m_Connections[i];
		}
	}

	return nullptr;
}

void Server::TerminateClient(Connection* client)
{
	auto it = std::find_if(m_Connections.begin(), m_Connections.end(),
		[client](Connection other) { return client->RemoteEndpoint == other.RemoteEndpoint; });

	if (it != m_Connections.end())
	{
		NetworkMessage msg(PacketType::Disconnect);
		client->Send(msg);
		m_Connections.erase(it);
	}
}
