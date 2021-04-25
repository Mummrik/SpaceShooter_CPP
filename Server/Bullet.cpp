#include "Bullet.h"

void Bullet::OnUpdate(Server* server, float deltaTime)
{
	if (IsActive == false)
		return;

	m_Position += m_Velocity * 0.075f;// *deltaTime;

	NetworkMessage msg(PacketType::MoveBullet);
	msg.Write(m_Uid);
	msg.Write(m_Position.x);
	msg.Write(m_Position.y);
	server->SendToAll(msg, false);

	m_Existance += deltaTime;
	if (m_Existance >= m_LifeTime)
	{
		IsActive = false;
		server->RemoveBullet.push(m_Uid);
	}
}

uint64_t Bullet::GetBulletUid()
{
	return m_Uid;
}

Vec2d Bullet::GetPosition()
{
	return m_Position;
}

Vec2d Bullet::GetVelocity()
{
	return m_Velocity;
}
