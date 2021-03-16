#include "RPC.h"

void RPC_Disconnect(Connection* client, NetworkMessage data)
{
	// Code for disconnecting a client...
	std::cout << ">> Server" << " => Called RPC_Disconnect()" << std::endl;
	client->IsConnected = false;
}

void RPC_HandShake(Connection* client, NetworkMessage data)
{
	//TODO: Set connection id
	uint64_t id = data.ReadUint64();

	// Code for client handshaking...
	std::cout << ">> Server" << " => Called RPC_HandShake() id: " << id << std::endl;

	std::cout << client->Endpoint << std::endl;
	NetworkMessage msg(PacketType::HandShake);
	msg.Send(client->Socket, client->Endpoint);
}

void RPC::Invoke(PacketType type, Connection* client, NetworkMessage data)
{
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
