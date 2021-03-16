#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>

using asio::ip::udp;

class Connection
{
public:
	Connection(udp::socket* socket, udp::endpoint ep) : Socket(socket), Endpoint(ep)
	{
	}

	bool SetId(/*size_t newId*/)
	{
		if (Id == 0)
		{
			Id = reinterpret_cast<size_t>(this);
			return true;
		}

		return false;
	}

public:
	udp::socket* Socket;
	udp::endpoint Endpoint;
	bool Authorized = false;
	size_t Id;
};