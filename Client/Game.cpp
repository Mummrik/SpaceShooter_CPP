#include "Game.h"
#include "Connection.h"

bool Game::OnUserCreate()
{
	// Called once at the start, so create things here
	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	//Clear(olc::BLANK);

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
