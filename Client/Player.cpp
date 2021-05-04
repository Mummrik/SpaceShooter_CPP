#include "Player.h"

void Player::OnUpdate(Game* game, float fElapsedTime)
{
	if (IsActive == false)
		return;


	Render(game, fElapsedTime);
}

uint64_t Player::GetPlayerUid()
{
	return m_Uid;
}

olc::vf2d Player::GetPlayerPosition()
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

void Player::SetRotation(float rotation)
{
	m_Rotation = rotation;
}

void Player::SetPosition(const olc::vf2d& position)
{
	m_Position = position;
}

void Player::SetTint(const olc::Pixel& tint, float duration)
{
	m_AnimateTimer = duration;
	m_Tint = tint;
}

void Player::SetHealth(float health)
{
	m_Health = health;
}

void Player::Render(Game* game, float fElapsedTime)
{
	if (IsThrusting)
	{
		IsThrusting = false;
		game->DrawPartialRotatedDecal
		(
			m_Position + game->GetCameraPosition(),
			game->Gfx.decal,
			m_Rotation,
			olc::vf2d(128.f, 128.f),
			olc::vf2d(256.f, 256.f * m_SpriteId),
			olc::vf2d(256.f, 256.f),
			olc::vf2d(0.25f, 0.25f)
		);
	}

	game->DrawPartialRotatedDecal
	(
		m_Position + game->GetCameraPosition(),
		game->Gfx.decal,
		m_Rotation,
		olc::vf2d(128.f, 128.f),
		olc::vf2d(0, 256.f * m_SpriteId),
		olc::vf2d(256.f, 256.f),
		olc::vf2d(0.25f, 0.25f),
		m_Tint
	);

	DrawHealthBar(game);

	if (m_AnimateTimer > 0)
	{
		m_AnimateTimer -= fElapsedTime;
	}
	else
	{
		m_Tint = olc::WHITE;
	}
}

void Player::DrawHealthBar(Game* game)
{
	if (m_Health > 0)
	{
		float percent = (m_Health / m_MaxHealth);
		//draw 2 times to get 2 pixel thickness
		for (float i = 0; i < 2; i++)
		{
			game->DrawLine
			(
				m_Position + game->GetCameraPosition() + olc::vf2d(-16 * percent, 32 + i),
				m_Position + game->GetCameraPosition() + olc::vf2d(16 * percent, 32 + i),
				percent > 0.66f ? olc::GREEN : percent > 0.33f ? olc::YELLOW : olc::RED
			);
		}
	}
}
