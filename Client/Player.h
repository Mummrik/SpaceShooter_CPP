#pragma once

#include <string>
#include <NetworkMessage.h>
#include "olcPixelGameEngine.h"

class Player
{
public:
	Player(size_t uid, uint8_t spriteId, const std::string& name) : m_Id(uid), m_Name(name), m_SpriteId(spriteId) {}

private:
	size_t m_Id;
	std::string m_Name;
	uint8_t m_SpriteId;

public:
	olc::vf2d Position;
	bool IsThusting = false;

public:
	void OnUpdate(olc::PixelGameEngine* Engine, Game* game, float fElapsedTime);
	const std::string& GetName() { return m_Name; }

private:
	void Render(olc::PixelGameEngine* Engine, Game* game);
};
