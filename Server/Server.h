#pragma once

#include <iostream>
#include <thread>
#include "RPC.h"

class Server
{
public:
	Server(const uint16_t port) : m_Port(port), m_Socket(m_Context, udp::endpoint(udp::v4(), port)) {}
	~Server() {}

	bool Start();
	bool Shutdown();
	bool IsRunning = false;

private:
	const uint16_t m_Port;
	asio::io_context m_Context;
	udp::socket m_Socket;
	std::thread m_Listener;
	RPC m_Rpc;
	std::vector<Connection> m_Connections;

public:
	std::thread* GetListenerThread() { return &m_Listener; }
	void OnHandle(std::vector<char> Data, udp::endpoint RemoteEndpoint);
	void OnListen();

public:
	Connection* GetConnection(udp::endpoint RemoteEndpoint);
	bool CompareEndpoints(udp::endpoint a, udp::endpoint b) { return a == b; }
	void TerminateClient(Connection* client);
};
