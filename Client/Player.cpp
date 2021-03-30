#include "Player.h"
#include "Game.h"

void Player::OnUpdate(olc::PixelGameEngine* Engine, Game* Game, float fElapsedTime)
{
	Render(Engine, Game);
}

void Player::Render(olc::PixelGameEngine* Engine, Game* Game)
{
	//TODO: Rotate decals to look at mouse cursor
	if (IsThusting)
	{
		Engine->DrawPartialDecal(Position + olc::vi2d(-32, -32), olc::vf2d(64, 64), Game->Gfx.decal, olc::vf2d(256, 256 * m_SpriteId), olc::vf2d(256, 256));
		IsThusting = false;
	}
	Engine->DrawPartialDecal(Position + olc::vi2d(-32,-32), olc::vf2d(64, 64), Game->Gfx.decal, olc::vf2d(0, 256 * m_SpriteId), olc::vf2d(256, 256));
}
