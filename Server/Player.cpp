#include "Player.h"

void Player::OnUpdate(Server* server, float deltaTime)
{
	if (IsActive == false)
		return;

	m_Position += m_Velocity * deltaTime;

	NetworkMessage msg(PacketType::Movement);
	msg.Write(m_Uid);
	msg.Write(m_Position.x);
	msg.Write(m_Position.y);
	server->SendToAll(msg, false);

	if (CanShoot() == false)
	{
		m_Cooldown -= deltaTime;
	}
}

uint64_t Player::GetPlayerUid()
{
	return m_Uid;
}

Vec2d Player::GetPlayerPosition()
{
	return m_Position;
}

uint8_t Player::GetPlayerSpriteId()
{
	return m_SpriteId;
}

float Player::GetPlayerRotation()
{
	return m_Rotation;
}

void Player::AddVelocity(const Vec2d& velocity)
{
	//TODO: add velocity, and clamp
	m_Velocity += velocity;
}

bool Player::CanShoot()
{
	return m_Cooldown <= 0;
}

void Player::SetCooldown(float cooldown)
{
	m_Cooldown = cooldown;
}
