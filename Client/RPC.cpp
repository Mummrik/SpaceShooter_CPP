#include "RPC.h"
#include "Connection.h"
#include "Game.h"

void RPC::Invoke(RPC& rpc, const PacketType& type, Connection* client, NetworkMessage& data)
{
	if ((size_t)type >= 0 && type < PacketType::MAX_LENGTH && m_Rpc[(size_t)type] != nullptr)
	{
		//std::cout << "Handle PacketType : " << (size_t)type << std::endl;
		std::invoke(m_Rpc[(size_t)type], rpc, client, data);
	}
}

void RPC::Initialize(Game* game)
{
	m_Game = game;

	//Register RPC's here
	m_Rpc[(size_t)PacketType::Disconnect] = &RPC::Disconnect;
	m_Rpc[(size_t)PacketType::HandShake] = &RPC::HandShake;
	m_Rpc[(size_t)PacketType::NewPlayer] = &RPC::NewPlayer;
	m_Rpc[(size_t)PacketType::Movement] = &RPC::Movement;
	m_Rpc[(size_t)PacketType::RemovePlayer] = &RPC::RemovePlayer;
}

void RPC::Disconnect(Connection* client, NetworkMessage& data)
{
	std::cout << ">> Server" << " => RPC::Disconnect()" << std::endl;
	client->bShutdown = true;
}

void RPC::HandShake(Connection* client, NetworkMessage& data)
{
	client->Id = data.ReadUint64();

	std::cout << ">> Server" << " => RPC::HandShake()\n\tcid: " << client->Id << std::endl;

	NetworkMessage msg(PacketType::HandShake);
	client->Send(msg);
}

void RPC::NewPlayer(Connection* client, NetworkMessage& data)
{
	std::cout << ">> Server" << " => RPC::NewPlayer()" << std::endl;

	size_t uid = data.ReadUint64();
	m_Game->AddPlayer(uid, new Player(uid, data.ReadUint8(), "PlayerName"));

	Player* player = m_Game->GetPlayer(uid);
	player->Position.x = (int)data.ReadInt64();
	player->Position.y = (int)data.ReadInt64();


}

void RPC::Movement(Connection* client, NetworkMessage& data)
{
	if (auto player = m_Game->GetPlayer(data.ReadUint64()))
	{
		player->Position.x = (int)data.ReadInt64();
		player->Position.y = (int)data.ReadInt64();
		player->IsThusting = true;
	}
}

void RPC::RemovePlayer(Connection* client, NetworkMessage& data)
{
	size_t uid = data.ReadUint64();
	m_Game->RemovePlayer(uid);
}
