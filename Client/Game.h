#pragma once

#include "olcPixelGameEngine.h"
#include "Player.h"

class Connection;

struct GraphicObj
{
	GraphicObj() {}
	~GraphicObj()
	{
		delete decal;
		delete sprite;
	}

	void Load(const std::string& path)
	{
		sprite = new olc::Sprite(path);
		decal = new olc::Decal(sprite);
	}

	olc::Sprite* sprite = nullptr;
	olc::Decal* decal = nullptr;
};

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

public:
	GraphicObj Gfx;

private:
	Connection* m_Client;
	std::map<size_t, Player*> m_Players;
	olc::vf2d m_CameraPosition = olc::vi2d((int)(ScreenWidth() * 0.5f), (int)(ScreenHeight() * 0.5f));
	olc::vf2d m_MousePosition;
	
public:
	void AddPlayer(size_t uid, Player* player);
	Player* GetPlayer(size_t uid);
	void RemovePlayer(size_t uid);

private:
	void Input();

};

