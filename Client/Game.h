#pragma once

#include "olcPixelGameEngine.h"
#include <sys/stat.h>
#include <queue>

class Connection;
class Player;

struct GraphicObj
{
	GraphicObj() {}
	~GraphicObj()
	{
		delete decal;
		delete sprite;
	}

	bool Load(const std::string& path)
	{
		struct stat buffer;
		if (stat(path.c_str(), &buffer) == 0)
		{
			sprite = new olc::Sprite(path);
			decal = new olc::Decal(sprite);
			return true;
		}
		else
			std::cout << "Error: Couldn't find path " << path << std::endl;

		return false;
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
	std::shared_ptr<olc::Sprite> sprite;
	std::queue<uint64_t> RemovePlayer;

private:
	Connection* m_Client;
	std::shared_ptr<Player> m_Player;
	olc::vf2d m_CameraPosition;
	std::array<std::shared_ptr<Player>, 1000> m_Players;

public:
	void DrawCrossair();
	float CalcRotation(const olc::vf2d& a, const olc::vf2d& b);
	const olc::vf2d& GetCameraPosition();
	void SetCameraPosition(const olc::vf2d& newPosition);
	void NewPlayer(uint64_t uid, uint8_t spriteId, olc::vf2d position, float rotation);
	void NewBullet(uint64_t uid, olc::vf2d position);
	size_t GetPlayerIndex(uint64_t uid);
	std::shared_ptr<Player> GetLastPlayerElement();
	std::shared_ptr<Player> GetPlayer(uint64_t uid);
	
private:
	void Input();
	uint16_t FindEmptyPlayerIndex();
	void CheckRemovePlayer();

};

