#pragma once

#include "olcPixelGameEngine.h"

class Connection;

class Game : public olc::PixelGameEngine
{
public:
	Game(const std::string& title, Connection* client) : m_Client(client)
	{
		sAppName = title;
	}

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	bool OnUserDestroy() override;

private:
	Connection* m_Client;

};

