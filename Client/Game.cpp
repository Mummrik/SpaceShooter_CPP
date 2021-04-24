#include "Game.h"
#include "Connection.h"
#include "Player.h"

bool Game::OnUserCreate()
{
	// Called once at the start, so create things here
	if (Gfx.Load("..\\Assets\\ships.png"))
	{
		m_Client->Start(this);
	}

	m_CameraPosition = GetWindowSize();
	m_CameraPosition *= 0.5f;

	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	Clear(olc::BLANK);

	DrawCrossair();
	Input();

	for (auto player : m_Players)
	{
		if (player == nullptr)
		{
			break;
		}
		player->OnUpdate(this);
	}

	CheckRemovePlayer();
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

const olc::vf2d& Game::GetCameraPosition()
{
	return m_CameraPosition;
}

void Game::SetCameraPosition(const olc::vf2d& newPosition)
{
	olc::vf2d offset = GetWindowSize();
	offset *= 0.5f;
	m_CameraPosition = newPosition + offset;
}

void Game::NewPlayer(uint64_t uid, uint8_t spriteId, olc::vf2d position, float rotation)
{
	uint16_t freeIndex = FindEmptyPlayerIndex();
	if (freeIndex > m_Players.size())
	{
		std::cout << "Warning: Couldn't add Player, maximum amount of players reached." << std::endl;
		return;
	}

	auto player = std::make_shared<Player>(uid, spriteId, position, rotation);
	m_Players[freeIndex] = player;

	player->IsActive = true;

	if (uid == m_Client->Id)
		m_Player = player;
}

void Game::NewBullet(uint64_t uid, olc::vf2d position)
{
}

size_t Game::GetPlayerIndex(uint64_t uid)
{
	size_t length = m_Players.size();
	for (size_t i = 0; i < length; i++)
	{
		auto player = m_Players[i];
		if (player->GetPlayerUid() == uid)
		{
			return i;
		}
	}
	return -1;
}

std::shared_ptr<Player> Game::GetLastPlayerElement()
{
	std::shared_ptr<Player> player = nullptr;
	size_t length = m_Players.size();
	for (size_t i = 0; i < length; i++)
	{
		if (m_Players[i] == nullptr)
			break;

		player = m_Players[i];
	}

	return player;
}

std::shared_ptr<Player> Game::GetPlayer(uint64_t uid)
{
	size_t length = m_Players.size();
	for (size_t i = 0; i < length; i++)
	{
		if (m_Players[i] == nullptr)
			return nullptr;

		if (m_Players[i]->GetPlayerUid() == uid)
			return m_Players[i];
	}

	return nullptr;
}

void Game::Input()
{
	float rotation = CalcRotation(m_CameraPosition + m_Player->GetPlayerPosition(), GetMousePos());
	if (rotation != m_Player->GetPlayerRotation())
	{
		NetworkMessage msg(PacketType::RotatePlayer);
		msg.Write(rotation);
		m_Client->Send(msg);
	}

	// Left mouse button
	if (GetMouse(0).bPressed)
	{
		olc::vf2d direction = GetMousePos() - (m_CameraPosition + m_Player->GetPlayerPosition());
		direction = direction.norm();

		if (direction == direction)
		{
			NetworkMessage msg(PacketType::FireBullet);
			msg.Write(direction.x);
			msg.Write(direction.y);
			m_Client->Send(msg);
		}
	}


	// Right mouse button
	if (GetMouse(1).bHeld)
	{
		olc::vf2d direction = GetMousePos() - (m_CameraPosition + m_Player->GetPlayerPosition());
		direction = direction.norm();

		if (direction == direction)
		{
			NetworkMessage msg(PacketType::Movement);
			msg.Write(direction.x);
			msg.Write(direction.y);
			m_Client->Send(msg);
		}
	}
}

uint16_t Game::FindEmptyPlayerIndex()
{
	auto length = m_Players.size();
	for (uint16_t i = 0; i < length; i++)
		if (m_Players[i] == nullptr)
			return i;

	return -1;
}

void Game::CheckRemovePlayer()
{
	if (size_t length = RemovePlayer.size() > 0)
	{
		for (size_t i = 0; i < length; i++)
		{
			uint64_t uid = RemovePlayer.front();
			RemovePlayer.pop();

			size_t removeIndex = GetPlayerIndex(uid);
			auto lastElement = GetLastPlayerElement();
			size_t lastElementIndex = GetPlayerIndex(lastElement->GetPlayerUid());

			if (removeIndex == lastElementIndex)
			{
				m_Players[removeIndex] = nullptr;
			}
			else
			{
				m_Players[removeIndex] = lastElement;
				m_Players[lastElementIndex] = nullptr;
			}
		}
	}
}

void Game::DrawCrossair()
{
	olc::vf2d mousePos = GetMousePos();
	uint8_t size = 10;
	DrawLine(mousePos + olc::vf2d(0, (float)(-size - 3)), mousePos + olc::vf2d(0, (float)(size + 3)), olc::RED);
	DrawLine(mousePos + olc::vf2d((float)(-size - 3), 0), mousePos + olc::vf2d((float)(size + 3), 0), olc::RED);
	DrawCircle(mousePos, size, olc::RED);
}

float Game::CalcRotation(const olc::vf2d& a, const olc::vf2d& b)
{
	return atan2f(b.y - a.y, b.x - a.x) - 80.13f;
}
