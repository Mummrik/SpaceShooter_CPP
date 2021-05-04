#include "RPC.h"
#include "Connection.h"
#include "Game.h"
#include "Player.h"
#include "Bullet.h"

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
	m_Rpc[(size_t)PacketType::RemovePlayer] = &RPC::RemovePlayer;
	m_Rpc[(size_t)PacketType::Movement] = &RPC::Movement;
	m_Rpc[(size_t)PacketType::RotatePlayer] = &RPC::Rotation;
	m_Rpc[(size_t)PacketType::AnimateJetEngine] = &RPC::AnimateJetEngine;
	m_Rpc[(size_t)PacketType::FireBullet] = &RPC::FireBullet;
	m_Rpc[(size_t)PacketType::MoveBullet] = &RPC::MoveBullet;
	m_Rpc[(size_t)PacketType::RemoveBullet] = &RPC::RemoveBullet;
	m_Rpc[(size_t)PacketType::Ping] = &RPC::Ping;
	m_Rpc[(size_t)PacketType::AnimateDamage] = &RPC::AnimateDamage;
	m_Rpc[(size_t)PacketType::KDStats] = &RPC::KDStats;
	m_Rpc[(size_t)PacketType::UpdateHealth] = &RPC::UpdateHealth;
}

void RPC::Disconnect(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> Server" << " => RPC::Disconnect()" << std::endl;
	client->bShutdown = true;
}

void RPC::HandShake(Connection* client, NetworkMessage& data)
{
	client->Id = data.ReadUint64();

	//std::cout << ">> Server" << " => RPC::HandShake()\n\tcid: " << client->Id << std::endl;

	NetworkMessage msg(PacketType::HandShake);
	client->Send(msg);
}

void RPC::NewPlayer(Connection* client, NetworkMessage& data)
{
	std::cout << ">> Server" << " => RPC::NewPlayer()" << std::endl;
	uint64_t uid = data.ReadUint64();
	uint8_t spriteId = data.ReadUint8();
	olc::vf2d position(data.ReadFloat(), data.ReadFloat());
	float rotation = data.ReadFloat();

	m_Game->NewPlayer(uid, spriteId, position, rotation);
}

void RPC::RemovePlayer(Connection* client, NetworkMessage& data)
{
	std::cout << ">> Server" << " => RPC::RemovePlayer()" << std::endl;
	m_Game->RemovePlayer.push(data.ReadUint64());
}

void RPC::Movement(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> Server" << " => RPC::Movement()" << std::endl;
	if (auto player = m_Game->GetPlayer(data.ReadUint64()))
	{
		player->SetPosition(olc::vf2d(data.ReadFloat(), data.ReadFloat()));
	}
}

void RPC::Rotation(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> Server" << " => RPC::Rotation()" << std::endl;
	if (auto player = m_Game->GetPlayer(data.ReadUint64()))
	{
		player->SetRotation(data.ReadFloat());
	}
}

void RPC::AnimateJetEngine(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> Server" << " => RPC::AnimateJetEngine()" << std::endl;
	if (auto player = m_Game->GetPlayer(data.ReadUint64()))
	{
		player->IsThrusting = true;
	}
}

void RPC::FireBullet(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> Server" << " => RPC::FireBullet()" << std::endl;
	uint64_t uid = data.ReadUint64();
	olc::vf2d position(data.ReadFloat(), data.ReadFloat());
	float rotation = data.ReadFloat();

	m_Game->NewBullet(uid, position, rotation);
}

void RPC::MoveBullet(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> Server" << " => RPC::MoveBullet()" << std::endl;
	if (auto bullet = m_Game->GetBullet(data.ReadUint64()))
	{
		bullet->SetPosition(olc::vf2d(data.ReadFloat(), data.ReadFloat()));
	}
}

void RPC::RemoveBullet(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> Server" << " => RPC::RemoveBullet()" << std::endl;
	uint64_t uid = data.ReadUint64();
	if (auto bullet = m_Game->GetBullet(uid))
	{
		bullet->IsActive = false;
		m_Game->RemoveBullet.push(uid);
	}
}

void RPC::Ping(Connection* client, NetworkMessage& data)
{
	//std::cout << ">> Server" << " => RPC::Ping()" << std::endl;
	int64_t time = data.ReadUint64();
	auto currentTime = std::chrono::system_clock::now().time_since_epoch().count();
	m_Game->Ping = (int)(time - (int64_t)currentTime) / 10000000;
}

void RPC::AnimateDamage(Connection* client, NetworkMessage& data)
{
	uint64_t uid = data.ReadUint64();
	if (auto player = m_Game->GetPlayer(uid))
	{
		player->SetTint(olc::RED, 0.1f);
		player->SetHealth(data.ReadFloat());
	}
}

void RPC::KDStats(Connection* client, NetworkMessage& data)
{
	m_Game->Kills = data.ReadUint32();
	m_Game->Deaths = data.ReadUint32();
}

void RPC::UpdateHealth(Connection* client, NetworkMessage& data)
{
	uint64_t uid = data.ReadUint64();
	if (auto player = m_Game->GetPlayer(uid))
	{
		player->SetTint(olc::GREEN, 0.1f);
		player->SetHealth(data.ReadFloat());
	}
}
