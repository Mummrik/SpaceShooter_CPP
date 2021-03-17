// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Client.h"

int main()
{
	// params host, port
	Client game("127.0.0.1", "7171");
	if (game.Construct(1280, 720, 1, 1))
		game.Start();

	game.GetListenerThread()->join();
	game.GetSocket()->close();

	return 0;
}

bool Client::Connect(udp::endpoint RemoteEndpoint)
{
	if (m_Client.IsConnected)
		return false;

	std::cout << "Try connecting | " << RemoteEndpoint << std::endl;

	m_Socket.open(udp::v4());
	NetworkMessage msg(PacketType::HandShake);
	msg.Send(&m_Socket, RemoteEndpoint);

	m_Client.Endpoint = RemoteEndpoint;
	m_Client.IsConnected = true;
	return true;
}

void Client::OnHandle(std::vector<char> Data)
{
	NetworkMessage msg(Data);
	m_Rpc.Invoke(msg.GetPacketType(), &m_Client, msg);
}

void Client::OnListen()
{
	std::cout << "Listener thread started" << std::endl;
	std::array<char, 1024> InBuffer;
	udp::endpoint RemoteEndpoint;

	while (m_Client.IsConnected)
	{
		size_t recvSize = m_Socket.receive_from(asio::buffer(InBuffer), RemoteEndpoint);

		if (recvSize > 0 && RemoteEndpoint == m_RemoteEndpoint)
		{
			std::vector<char> data;
			for (size_t i = 0; i < recvSize; i++)
				data.push_back(InBuffer[i]);

			OnHandle(data);
		}
	}

	std::cout << "Listener thread terminated" << std::endl;
}
