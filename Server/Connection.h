#pragma once

#include "NetworkMessage.h"
#include "Player.h"

class Connection
{
public:
	Connection(udp::socket* socket, const udp::endpoint& ep) : Socket(socket), Endpoint(ep) {}

	bool SetId()
	{
		if (Id == 0)
			Id = reinterpret_cast<size_t>(this);

		return Id != 0;
	}

public:
	udp::socket* Socket;
	udp::endpoint Endpoint;
	bool Authorized = false;
	size_t Id = 0;
	Player* Player;
};