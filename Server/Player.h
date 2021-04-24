#pragma once
#include "Server.h"

class Player
{
public:
	Player(uint64_t uid, uint8_t spriteId) : m_Uid(uid), m_SpriteId(spriteId) {}

private:
	uint64_t m_Uid;
	Vec2d m_Position;
	float m_Rotation = 0;
	Vec2d m_Velocity;
	uint8_t m_SpriteId;

public:
	bool IsActive = false;
	void OnUpdate(Server* server, float deltaTime);
	uint64_t GetPlayerUid();
	Vec2d GetPlayerPosition();
	uint8_t GetPlayerSpriteId();
	float GetPlayerRotation();
	void AddVelocity(const Vec2d& velocity);
};
