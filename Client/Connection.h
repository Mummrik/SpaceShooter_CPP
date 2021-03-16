#pragma once

class Connection
{
public:
	Connection(udp::socket* socket) : Socket(socket)
	{
	}

public:
	udp::socket* Socket;
	udp::endpoint Endpoint;
	bool Authorized = false;
	bool IsConnected = false;
	size_t Id;
};