#include "RPC.h"

void RPC::Invoke(PacketType type, Client* client, NetworkMessage data)
{
	auto it = m_RpcMap.find(type);
	if (it != m_RpcMap.end())
	{
		m_RpcMap[type];
	}
}

void RPC::Init()
{
	/*m_RpcMap.emplace(PacketType::Disconnect, &RPC::RPC_Disconnect);
	m_RpcMap.emplace(PacketType::HandShake, &RPC::RPC_HandShake);*/
}

void RPC::RPC_Disconnect(Client* client, NetworkMessage data)
{
	// Code for disconnecting a client...
	std::cout << ">> Server " << " => Called RPC_Disconnect()" << std::endl;
	/*NetworkMessage msg(PacketType::Disconnect);
	msg.Send(owner.Socket, owner.RemoteEndpoint);*/

}

void RPC::RPC_HandShake(Client* client, NetworkMessage data)
{
	//TODO: Set connection id
	uint64_t id = data.ReadUint64();

	// Code for client handshaking...
	std::cout << ">> Server " << " => Called RPC_HandShake() id: " << id << std::endl;

	NetworkMessage msg(PacketType::HandShake);
	//msg.Send(client->GetSocket(),)
}