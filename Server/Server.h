#pragma once

#include <iostream>
#include "Connection.h"
#include "RPC.h"

class Server
{
public:
	Server(uint16_t port) : m_Socket(m_Context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), m_Rpc(this)
	{
		std::cout << "Server starting on port " << port << std::endl;
		Start();
	}
	~Server() { Shutdown(); }

	bool Start();
	bool Shutdown();
	bool IsRunning = false;

private:
	asio::io_context m_Context;
	asio::ip::udp::socket m_Socket;
	std::thread m_Listener;
	RPC m_Rpc;
	std::vector<Connection> m_Connections;

private:
	void OnHandle(const std::vector<char>& Data, const asio::ip::udp::endpoint& RemoteEndpoint);
	void OnListen();

public:
	Connection* GetConnection(const asio::ip::udp::endpoint& RemoteEndpoint);
	void TerminateClient(Connection* client);
};
