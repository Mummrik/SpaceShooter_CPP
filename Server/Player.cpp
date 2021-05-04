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
	if ((m_Velocity + velocity).Magnitude() <= 500)
	{
		m_Velocity += velocity;
	}
}

bool Player::CanShoot()
{
	return m_Cooldown <= 0;
}

void Player::SetCooldown(float cooldown)
{
	m_Cooldown = cooldown;
}

float Player::GetCollisionRadius()
{
	return m_CollisionRadius;
}

void Player::Damage(float damage, Server* server, Player* attacker)
{
	m_Health -= damage;

	if (m_Health <= 0)
	{
		if (attacker != nullptr)
		{
			attacker->AddKill(server);
		}

		OnDeath();

		NetworkMessage msg(PacketType::KDStats);
		msg.Write(m_Kills);
		msg.Write(m_Deaths);
		server->GetConnection(m_Uid)->Send(msg, true);
	}
}

void Player::AddKill(Server* server)
{
	m_Kills++;

	if (m_Health < m_MaxHealth)
	{
		m_Health += 25;
		if (m_Health > m_MaxHealth)
		{
			m_Health = m_MaxHealth;
		}

		NetworkMessage msg(PacketType::UpdateHealth);
		msg.Write(m_Uid);
		msg.Write(m_Health);
		server->SendToAll(msg, true);
	}

	NetworkMessage msg(PacketType::KDStats);
	msg.Write(m_Kills);
	msg.Write(m_Deaths);
	server->GetConnection(m_Uid)->Send(msg, true);
}

float Player::GetHealth()
{
	return m_Health;
}

void Player::OnDeath()
{
	m_Deaths++;
	m_Health = m_MaxHealth;
	m_Position = Vec2d();
	m_Rotation = 0;
	m_Velocity = Vec2d();
	m_Cooldown = 0;
}
