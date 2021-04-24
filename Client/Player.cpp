#include "Player.h"

void Player::OnUpdate(Game* game)
{
	if (IsActive == false)
		return;


	Render(game);
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

void Player::Render(Game* game)
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
		olc::vf2d(0.25f, 0.25f)
	);
}
