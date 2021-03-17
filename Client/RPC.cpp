#include "RPC.h"

static void RPC_Disconnect(Connection* client, NetworkMessage data)
{
	std::cout << ">> Server" << " => Called RPC_Disconnect()" << std::endl;
	client->IsConnected = false;
}

static void RPC_HandShake(Connection* client, NetworkMessage data)
{
	client->Id = data.ReadUint64();

	std::cout << ">> Server" << " => Called RPC_HandShake()\n\tcid: " << client->Id << std::endl;

	NetworkMessage msg(PacketType::HandShake);
	msg.Send(client->Socket, client->Endpoint);
}

void RPC::Init()
{
	//Register RPC's here!!!
	RegisterRPC(PacketType::Disconnect, RPC_Disconnect);
	RegisterRPC(PacketType::HandShake, RPC_HandShake);
}

void RPC::Invoke(const PacketType& type, Connection* client, const NetworkMessage& data)
{
	auto it = m_RpcMap.find(type);
	if (it != m_RpcMap.end())
	{
		it->second(client, data);
	}
}

void RPC::RegisterRPC(const PacketType& type, void (*func)(Connection*, const NetworkMessage))
{
	m_RpcMap.emplace(type, [func](Connection* client, const NetworkMessage& msg) { func(client, msg); });
}