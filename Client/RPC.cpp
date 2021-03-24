#include "RPC.h"
#include "Connection.h"

void RPC::Invoke(RPC& rpc, const PacketType& type, Connection* client, NetworkMessage& data)
{
	if ((size_t)type >= 0 && type < PacketType::MAX_LENGTH && m_Rpc[(size_t)type] != nullptr)
	{
		std::invoke(m_Rpc[(size_t)type], rpc, client, data);
	}
}

void RPC::Init()
{
	//Register RPC's here
	m_Rpc[(size_t)PacketType::Disconnect] = &RPC::Disconnect;
	m_Rpc[(size_t)PacketType::HandShake] = &RPC::HandShake;
	m_Rpc[(size_t)PacketType::NewPlayer] = &RPC::NewPlayer;
}

void RPC::Disconnect(Connection* client, NetworkMessage& data)
{
	std::cout << ">> Server" << " => Called RPC_Disconnect()" << std::endl;
	client->IsConnected = false;
}

void RPC::HandShake(Connection* client, NetworkMessage& data)
{
	client->Id = data.ReadUint64();

	std::cout << ">> Server" << " => Called RPC_HandShake()\n\tcid: " << client->Id << std::endl;

	NetworkMessage msg(PacketType::HandShake);
	client->Send(msg);
}

void RPC::NewPlayer(Connection* client, NetworkMessage& data)
{
	std::cout << ">> Server" << " => Called RPC_NewPlayer()" << std::endl;

	/*client->Player.Create("Player: " + std::to_string(client->Id));
	std::cout << client->Player.GetName() << std::endl;*/
}
