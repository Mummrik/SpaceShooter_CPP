#pragma once

#include <iostream>
#include "Connection.h"
#include "RPC.h"
#include <map>
#include <chrono>
#include "Player.h"

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
	std::thread m_Game;
	RPC m_Rpc;
	std::vector<Connection> m_Connections;

public:
	std::map<size_t, Player*> m_Players;

private:
	void OnHandle(const std::vector<char>& Data, const asio::ip::udp::endpoint& RemoteEndpoint);
	void OnListen();

public:
	void SendToAll(NetworkMessage& msg, bool reliable);
	Connection* GetConnection(const asio::ip::udp::endpoint& RemoteEndpoint);
	void TerminateClient(Connection* client);
	void RemovePlayer(size_t id);
	Player* GetPlayer(size_t uid);
};
