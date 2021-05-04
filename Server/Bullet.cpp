#include "Bullet.h"
#include "Player.h"

void Bullet::OnUpdate(Server* server, float deltaTime)
{
	if (IsActive == false)
		return;


	m_Position += m_Velocity * deltaTime * m_Speed;

	NetworkMessage msg(PacketType::MoveBullet);
	msg.Write(m_Uid);
	msg.Write(m_Position.x);
	msg.Write(m_Position.y);
	server->SendToAll(msg, false);

	m_Existance += deltaTime;
	if (m_Existance >= m_LifeTime)
		Destroy(server);

	CheckCollision(server);
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

float Bullet::GetRotation()
{
	return m_Rotation;
}

float Bullet::GetCollisionRadius()
{
	return m_CollisionRadius;
}

void Bullet::CheckCollision(Server* server)
{
	if (IsActive == false)
		return;

	for (auto player : server->GetPlayers())
	{
		if (player == nullptr)
			break;

		if (player->GetPlayerUid() != m_Owner &&
			m_Position.Distance(player->GetPlayerPosition()) < m_CollisionRadius + player->GetCollisionRadius())
		{
			Destroy(server);
			player->Damage(10, server, server->GetPlayer(m_Owner).get());

			NetworkMessage msg(PacketType::AnimateDamage);
			msg.Write(player->GetPlayerUid());
			msg.Write(player->GetHealth());
			server->SendToAll(msg, false);
			break;
		}
	}
}

void Bullet::Destroy(Server* server)
{
	IsActive = false;
	server->RemoveBullet.push(m_Uid);
}
