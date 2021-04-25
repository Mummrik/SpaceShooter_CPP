#include "Bullet.h"

void Bullet::Render(Game* game)
{
	if (IsActive == false)
		return;

	game->DrawPartialRotatedDecal
	(
		m_Position + game->GetCameraPosition(),
		game->Gfx.decal,
		m_Rotation,
		olc::vf2d(128.f, 128.f),
		olc::vf2d(256.f, 256.f * 5),
		olc::vf2d(256.f, 256.f),
		olc::vf2d(0.25f, 0.25f)
	);
}

uint64_t Bullet::GetBulletUid()
{
	return m_Uid;
}

void Bullet::SetPosition(const olc::vf2d& position)
{
	m_Position = position;
}
