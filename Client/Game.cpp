#include "Game.h"
#include "Connection.h"

bool Game::OnUserCreate()
{
	// Called once at the start, so create things here
	Gfx.Load("..\\assets\\ships.png");

	m_Client->Start(this);
	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	Clear(olc::BLANK);

	for (auto player : m_Players)
	{
		player.second->OnUpdate(this, this, fElapsedTime);
	}

	Input();

	return true;
}

bool Game::OnUserDestroy()
{
	// called when application close

	// Let the server know you exited the game
	NetworkMessage msg(PacketType::Disconnect);
	m_Client->Send(msg);

	return true;
}

void Game::AddPlayer(size_t uid, Player* player)
{
	m_Players.emplace(uid, player);
}

Player* Game::GetPlayer(size_t uid)
{
	auto it = std::find_if(m_Players.begin(), m_Players.end(),
		[&uid](std::pair<size_t, Player*> const& player) { return uid == player.first; });

	if (it != m_Players.end())
	{
		return m_Players[uid];
	}

	return nullptr;
}

void Game::RemovePlayer(size_t uid)
{
	Player* player = GetPlayer(uid);
	auto it = std::find_if(m_Players.begin(), m_Players.end(),
		[player](auto other) { return player == other.second; });

	if (it != m_Players.end())
	{
		m_Players.erase(it);
		delete player;
	}
}

void Game::Input()
{
	m_MousePosition = olc::vf2d(GetMouseX(), GetMouseY());

	// Left mouse button
	if (GetMouse(0).bHeld)
	{
		//TODO: Send firebullet
	}

	// Right mouse button
	if (GetMouse(1).bHeld)
	{
		olc::vf2d direction = m_MousePosition - GetPlayer(m_Client->Id)->Position;
		direction = direction.norm();

		NetworkMessage msg(PacketType::Movement);
		if (direction.x > 0)
			msg.Write((int8_t)direction.ceil().x);
		else
			msg.Write((int8_t)direction.floor().x);

		if (direction.y > 0)
			msg.Write((int8_t)direction.ceil().y);
		else
			msg.Write((int8_t)direction.floor().y);

		m_Client->Send(msg);
	}
}
