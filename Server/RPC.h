#pragma once

#include <iostream>
#include <map>
#include "NetworkMessage.h"
#include "Connection.h"

class RPC
{
public:
	RPC() { Init(); }
	void Invoke(PacketType type, Connection* client, NetworkMessage data);

private:
	void Init();
	std::map<PacketType, std::function<void(Connection*, NetworkMessage&)>> m_RpcMap;
	void RegisterRPC(PacketType type, void (*func)(Connection*, NetworkMessage))
	{
		m_RpcMap.emplace(type, [func](Connection* client, NetworkMessage& msg) { func(client, msg); });
	}
};

