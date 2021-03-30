#pragma once

#include <iostream>
#include <NetworkMessage.h>

class Connection;

struct Vec2d
{
	int x;
	int y;

	Vec2d(int x, int y) : x(x), y(y) {}
};

class Player
{
public:
	Player(Connection* client, uint8_t spriteId) : client(client), Position(100, 100), spriteId(spriteId) {}

public:
	Connection* client;
	Vec2d Position;
	uint8_t spriteId;

};