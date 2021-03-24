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
}

void RPC::Disconnect(Connection* client, NetworkMessage& data)
{
	// Code for disconnecting a client...
	std::cout << ">> Called RPC_Disconnect()" << std::endl;

	m_Server->TerminateClient(client);
}

void RPC::HandShake(Connection* client, NetworkMessage& data)
{
	// Code for client handshaking...
	std::cout << ">> Called RPC_HandShake()" << std::endl;
	client->Authorized = true;

	NetworkMessage msg(PacketType::NewPlayer);
	client->Send(msg);
}
