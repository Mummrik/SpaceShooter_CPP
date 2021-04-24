#include "Connection.h"

void Connection::Start(Game* game)
{
	if (m_Socket.is_open())
		return;

	m_Rpc.Initialize(game);

	std::cout << "Try connecting | " << m_RemoteEndpoint << std::endl;

	m_Socket.open(asio::ip::udp::v4());
	m_Listener = std::thread(&Connection::OnListen, this);
}

void Connection::Shutdown()
{
	if (m_Socket.is_open())
		m_Socket.close();

	if (m_Listener.joinable())
		m_Listener.join();
}

void Connection::Send(NetworkMessage& msg)
{
	msg.PrepareSend();
	m_Socket.send_to(asio::buffer(msg.Body), m_RemoteEndpoint);
}

void Connection::OnListen()
{
	std::cout << "Listener thread initialized" << std::endl;
	std::array<char, 1024> InBuffer;
	asio::ip::udp::endpoint RemoteEndpoint;
	size_t recvSize = 0;

	// Send dummy data to setup network
	m_Socket.send_to(asio::buffer({ (uint8_t)0 }), m_RemoteEndpoint);

	while (!bShutdown)
	{
		try
		{
			recvSize = m_Socket.receive_from(asio::buffer(InBuffer), RemoteEndpoint);
		}
		catch (const std::exception& ex)
		{
			std::cout << "\nException: " << ex.what() << "\n" << std::endl;
			//break;
		}

		if (recvSize > 0 && RemoteEndpoint == m_RemoteEndpoint)
		{
			std::vector<char> data(InBuffer.begin(), InBuffer.begin() + recvSize);
			OnHandle(data);
		}
	}

	std::cout << "Listener thread terminated" << std::endl;
}

void Connection::OnHandle(const std::vector<char>& Data)
{
	NetworkMessage msg(Data);
	m_Rpc.Invoke(m_Rpc, msg.GetPacketType(), this, msg);
}
