#include "Player.h"

void Player::Create(const std::string& name)
{
	m_Name = name;
	//m_Position = olc::vf2d(10, 10);
}

void Player::OnUpdate(/*olc::PixelGameEngine* Engine,*/ float fElapsedTime)
{
	/*Engine->DrawCircle(m_Position, 10);
	m_Position.x += 0.01f;*/
}
