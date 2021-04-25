#pragma once
#include "Game.h"

class Bullet
{
public:
	Bullet(uint64_t uid, olc::vf2d position, float rotation) : m_Uid(uid), m_Position(position), m_Rotation(rotation) {}

private:
	uint64_t m_Uid = 0;
	olc::vf2d m_Position;
	float m_Rotation;

public:
	bool IsActive = false;
	void Render(Game* game);
	uint64_t GetBulletUid();
	void SetPosition(const olc::vf2d& position);
};

