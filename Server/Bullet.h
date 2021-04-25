#pragma once
#include "Server.h"

class Bullet
{
public:
	Bullet(Vec2d position, Vec2d velocity) : m_Uid(reinterpret_cast<uint64_t>(this)), m_Position(position), m_Velocity(velocity) {}

private:
	uint64_t m_Uid = 0;
	Vec2d m_Position;
	Vec2d m_Velocity;
	float m_LifeTime = 3.0f;
	float m_Existance = 0;

public:
	bool IsActive = false;
	void OnUpdate(Server* server, float deltaTime);
	uint64_t GetBulletUid();
	Vec2d GetPosition();
	Vec2d GetVelocity();

};