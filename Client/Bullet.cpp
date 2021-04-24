#include "Bullet.h"
#include "Game.h"

void Bullet::Render(Game* game)
{
	game->DrawPartialRotatedDecal
	(
		Position + game->GetCameraPosition(),
		game->Gfx.decal,
		Rotation,
		olc::vi2d(128, 128),
		olc::vf2d(256.f, 1280.f),
		olc::vi2d(256, 256),
		olc::vf2d(0.25f, 0.25f)
	);
}
