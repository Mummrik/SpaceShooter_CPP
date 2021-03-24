#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <NetworkMessage.h>
#include <iostream>
#include <string>
#include "RPC.h"

class Connection
{
public:
	Connection(const asio::ip::address& host, uint16_t port) : m_Socket(m_Context), m_RemoteEndpoint(host, port) {}
	~Connection() { Shutdown(); }

public:
	bool Authorized = false;
	bool IsConnected = false;
	size_t Id = 0;

public:
	void Start();
	void Shutdown();
	void Send(NetworkMessage& msg);

private:
	asio::io_context m_Context;
	asio::ip::udp::socket m_Socket;
	asio::ip::udp::endpoint m_RemoteEndpoint;
	std::thread m_Listener;
	RPC m_Rpc;

private:
	void OnListen();
	void OnHandle(const std::vector<char>& Data);
};