#pragma once

#include <iostream>
#include <array>
#include <NetworkMessage.h>

class Connection;

class RPC
{
public:
	RPC() { Init(); }
	void Invoke(RPC& rpc, const PacketType& type, Connection* client, NetworkMessage& data);

private:
	typedef void(RPC::* RpcCallbacks)(Connection*, NetworkMessage&);
	RpcCallbacks m_Rpc[(size_t)PacketType::MAX_LENGTH];

private:
	void Init();
	void Disconnect(Connection* client, NetworkMessage& data);
	void HandShake(Connection* client, NetworkMessage& data);
	void NewPlayer(Connection* client, NetworkMessage& data);

};

