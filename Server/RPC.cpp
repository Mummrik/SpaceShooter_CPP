#include "RPC.h"
#include "Connection.h"
#include "Server.h"

void RPC::Invoke(RPC& rpc, const PacketType& type, Connection* client, NetworkMessage& data)
{
	if ((uint16_t)type >= 0 && type < PacketType::MAX_LENGTH && m_Rpc[(size_t)type] != nullptr)
	{
		std::invoke(m_Rpc[(size_t)type], rpc, client, data);
	}
}

void RPC::Init()
{
	//Register RPC's here
	m_Rpc[(size_t)PacketType::Disconnect] = &RPC::Disconnect;
	m_Rpc[(size_t)PacketType::HandShake] = &RPC::HandShake;
	m_Rpc[(size_t)PacketType::Movement] = &RPC::Movement;
}

void RPC::Disconnect(Connection* client, NetworkMessage& data)
{
	// Code for disconnecting a client...
	std::cout << ">> " << client->RemoteEndpoint << " => RPC::Disconnect()" << std::endl;

	//m_Server->RemovePlayer(client->Id);
	m_Server->TerminateClient(client);
}

void RPC::HandShake(Connection* client, NetworkMessage& data)
{
	// Code for client handshaking...
	std::cout << ">> " << client->RemoteEndpoint << " => RPC::HandShake()" << std::endl;
	client->Authorized = true;

	for (auto& p : m_Server->m_Players)
	{
		auto& player = p.second;
		NetworkMessage msg(PacketType::NewPlayer);
		msg.Write(player->client->Id);
		msg.Write(player->spriteId);
		msg.Write((int64_t)player->Position.x);
		msg.Write((int64_t)player->Position.y);
		client->Send(msg, true);
	}

	m_Server->m_Players.emplace(client->Id, new Player(client, (uint8_t)(rand() % 10)));
	auto& player = m_Server->m_Players[client->Id];

	NetworkMessage msg(PacketType::NewPlayer);
	msg.Write(player->client->Id);
	msg.Write(player->spriteId);
	msg.Write((int64_t)player->Position.x);
	msg.Write((int64_t)player->Position.y);
	m_Server->SendToAll(msg, true);
}

void RPC::Movement(Connection* client, NetworkMessage& data)
{
	if (Player* player = m_Server->GetPlayer(client->Id))
	{
		player->Position.x += data.ReadInt8();
		player->Position.y += data.ReadInt8();

		NetworkMessage msg(PacketType::Movement);
		msg.Write(client->Id);
		msg.Write((int64_t)player->Position.x);
		msg.Write((int64_t)player->Position.y);
		m_Server->SendToAll(msg, false);
	}
}
