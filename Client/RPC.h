#pragma once

#include <iostream>
#include <map>
#include "NetworkMessage.h"
#include "Connection.h"

class Client;

class RPC
{
public:
	RPC() { Init(); }
	void Invoke(PacketType type, Connection* client, NetworkMessage data);

private:
	std::map<PacketType, std::function<void(Connection*, NetworkMessage&)>> m_RpcMap;
	void Init();
	void RegisterRPC(PacketType type, void (*func)(Connection*, NetworkMessage));

};

