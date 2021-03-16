#include "RPC.h"


void RPC_Disconnect(Connection client, NetworkMessage data)
{
	// Code for disconnecting a client...
	std::cout << ">> " << client.Endpoint << " => Called RPC_Disconnect()" << std::endl;

	NetworkMessage msg(PacketType::Disconnect);
	msg.Send(client.Socket, client.Endpoint);
}

void RPC_HandShake(Connection client, NetworkMessage data)
{
	// Code for client handshaking...
	std::cout << ">> " << client.Endpoint << " => Called RPC_HandShake()" << std::endl;
	client.Authorized = true;
}

void RPC::Init()
{
	// Add new RPC callbacks here
	m_RpcVector.push_back((void* (*)(void*))RPC_Disconnect);
	m_RpcVector.push_back((void* (*)(void*))RPC_HandShake);
}
