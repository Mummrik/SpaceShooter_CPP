#pragma once

#include <iostream>
#include <array>
#include <NetworkMessage.h>

class Server;
class Connection;

class RPC
{
public:
	RPC(Server* server) : m_Server(server) { Init(); }
	void Invoke(RPC& rpc, const PacketType& type, Connection* client, NetworkMessage& data);

private:
	typedef void(RPC::* RpcCallbacks)(Connection*, NetworkMessage&);
	RpcCallbacks m_Rpc[(size_t)PacketType::MAX_LENGTH];
	Server* m_Server;

private:
	void Init();
	void Disconnect(Connection* client, NetworkMessage& data);
	void HandShake(Connection* client, NetworkMessage& data);
	void Movement(Connection* client, NetworkMessage& data);
	void Rotation(Connection* client, NetworkMessage& data);
	void FireBullet(Connection* client, NetworkMessage& data);
	void Ping(Connection* client, NetworkMessage& data);
};


