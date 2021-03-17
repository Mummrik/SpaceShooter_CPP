// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Server.h"

int main()
{
	const uint16_t port(7171);
	Server server(port);
	server.Start();

	server.GetListenerThread()->join();
	server.Shutdown();

	return 0;
}

bool Server::Start()
{
	std::cout << "Server starting on port " << m_Port << std::endl;
	m_Listener = std::thread(&Server::OnListen, this);
	return true;
}

bool Server::Shutdown()
{
	IsRunning = false;
	m_Socket.close();
	return true;
}

void TestNetworkMessage()
{
	NetworkMessage testMsg(PacketType::Length);
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
	std::cout << "\n-- Test Message --\nMessageType: " << data.GetPacketType() << std::endl;
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
	IsRunning = true;
	std::cout << "Listener thread started" << std::endl;
	udp::endpoint RemoteEndpoint;
	std::array<char, 1024> InBuffer;

	// Testing NetworkMessage 
	//TestNetworkMessage();

	while (IsRunning)
	{
		size_t recvSize = m_Socket.receive_from(asio::buffer(InBuffer), RemoteEndpoint);

		if (recvSize > 0)
		{
			std::vector<char> data;
			for (size_t i = 0; i < recvSize; i++)
				data.push_back(InBuffer[i]);

			OnHandle(data, RemoteEndpoint);
		}
	}

	std::cout << "Listener thread terminated" << std::endl;
}

void Server::OnHandle(const std::vector<char>& Data, const udp::endpoint& RemoteEndpoint)
{
	if (Connection* client = GetConnection(RemoteEndpoint))
	{
		NetworkMessage msg(Data);
		if (client->Authorized)
		{
			m_Rpc.Invoke(msg.GetPacketType(), client, msg);
		}
		else if (!client->Authorized && msg.GetPacketType() == PacketType::HandShake)
		{
			m_Rpc.Invoke(msg.GetPacketType(), client, msg);
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

		Connection* c = &m_Connections[m_Connections.size() - 1];
		if (c->SetId())
		{
			std::cout << "New Connection | " << c->Endpoint << std::endl;

			NetworkMessage msg(PacketType::HandShake);
			msg.Write(c->Id);
			msg.Send(&m_Socket, RemoteEndpoint);
		}
		else
		{
			TerminateClient(c);
		}
	}
}

Connection* Server::GetConnection(const udp::endpoint& RemoteEndpoint)
{
	size_t size = m_Connections.size();
	for (size_t i = 0; i < size; i++)
	{
		if (CompareEndpoints(m_Connections[i].Endpoint, RemoteEndpoint))
		{
			return &m_Connections[i];
		}
	}

	return nullptr;
}

void Server::TerminateClient(Connection* client)
{
	// Step backwards to speed up terminating unvalid clients
	size_t lastIndex = m_Connections.size() - 1;
	for (auto i = lastIndex; i >= 0; --i)
	{
		if (client->Endpoint == m_Connections[i].Endpoint)
		{
			NetworkMessage msg(PacketType::Disconnect);
			msg.Send(&m_Socket, client->Endpoint);

			std::cout << "Terminated Connection | " << client->Endpoint << std::endl;

			m_Connections.erase(m_Connections.begin() + i);
			break;
		}
	}
}
