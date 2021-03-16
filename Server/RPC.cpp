#include "RPC.h"

void RPC_Disconnect(Connection* client, NetworkMessage data)
{
	// Code for disconnecting a client...
	std::cout << ">> " << client->Endpoint << " => Called RPC_Disconnect()" << std::endl;

	NetworkMessage msg(PacketType::Disconnect);
	msg.Send(client->Socket, client->Endpoint);
}

void RPC_HandShake(Connection* client, NetworkMessage data)
{
	// Code for client handshaking...
	std::cout << ">> " << client->Endpoint << " => Called RPC_HandShake()" << std::endl;
	client->Authorized = true;
}

void RPC::Invoke(PacketType type, Connection* client, NetworkMessage data)
{
	std::cout << client->Endpoint << std::endl;
	auto it = m_RpcMap.find(type);
	if (it != m_RpcMap.end())
	{
		it->second(client, data);
	}
}

void RPC::Init()
{
	//Register RPC's here!!!
	RegisterRPC(PacketType::Disconnect, RPC_Disconnect);
	RegisterRPC(PacketType::HandShake, RPC_HandShake);
}
