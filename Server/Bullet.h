#pragma once
#include "Server.h"

class Bullet
{
public:
	Bullet(uint64_t ownerId, Vec2d position, Vec2d velocity, float rotation) : m_Owner(ownerId), m_Uid(reinterpret_cast<uint64_t>(this)), m_Position(position), m_Velocity(velocity), m_Rotation(rotation) {}

private:
	uint64_t m_Uid = 0;
	uint64_t m_Owner = 0;
	Vec2d m_Position;
	Vec2d m_Velocity;
	float m_Rotation;
	float m_LifeTime = 3.0f;
	float m_Existance = 0;
	float m_CollisionRadius = 5;
	float m_Speed = 700;

public:
	bool IsActive = false;
	void OnUpdate(Server* server, float deltaTime);
	uint64_t GetBulletUid();
	Vec2d GetPosition();
	Vec2d GetVelocity();
	float GetRotation();
	float GetCollisionRadius();
	void CheckCollision(Server* server);
	void Destroy(Server* server);

};