#include "RPC.h"

static void RPC_Disconnect(Connection* client, NetworkMessage data)
{
	// Code for disconnecting a client...
	std::cout << ">> " << client->Endpoint << " => Called RPC_Disconnect()" << std::endl;
	client->Player->RemovePlayer();

	NetworkMessage msg(PacketType::Disconnect);
	msg.Send(client->Socket, client->Endpoint);
}

static void RPC_HandShake(Connection* client, NetworkMessage data)
{
	// Code for client handshaking...
	std::cout << ">> " << client->Endpoint << " => Called RPC_HandShake()" << std::endl;
	client->Authorized = true;

	// TODO: Setup player on server side
	client->Player->CreateNewPlayer();
	// TODO: send new player package to client(s)
}

void RPC::Init()
{
	//Register RPC's here!!!
	RegisterRPC(PacketType::Disconnect, RPC_Disconnect);
	RegisterRPC(PacketType::HandShake, RPC_HandShake);
}

void RPC::RegisterRPC(PacketType type, void (*func)(Connection*, NetworkMessage))
{
	m_RpcMap.emplace(type, [func](Connection* client, NetworkMessage& msg) { func(client, msg); });
}

void RPC::Invoke(PacketType type, Connection* client, NetworkMessage data)
{
	auto it = m_RpcMap.find(type);
	if (it != m_RpcMap.end())
	{
		it->second(client, data);
	}
}