#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <NetworkMessage.h>

class Connection
{
public:
	Connection(asio::ip::udp::socket* socket, const asio::ip::udp::endpoint& ep) : Socket(socket), RemoteEndpoint(ep), Id(reinterpret_cast<uint64_t>(this)) {}

public:
	asio::ip::udp::socket* Socket;
	asio::ip::udp::endpoint RemoteEndpoint;
	bool Authorized = false;
	uint64_t Id = 0;

public:
	void Send(NetworkMessage& msg, bool reliable);
};