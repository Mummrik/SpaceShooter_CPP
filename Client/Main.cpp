// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Game.h"
#include "Connection.h"
#include "RPC.h"

using asio::ip::address;

int main()
{
	address host = address::from_string("127.0.0.1");
	uint16_t port = 7171;

	Connection client(host, port);
	Game game("Title", &client);
	if (game.Construct(1280, 720, 1, 1))
	{
		game.Start();
	}

	return 0;
}
