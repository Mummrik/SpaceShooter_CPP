#pragma once

#include <iostream>
#include "NetworkMessage.h"
#include <map>

class RPC
{
public:
	RPC() { Init(); }
	void Invoke(PacketType type, class Client* client, NetworkMessage data);

private:
	std::map<PacketType, void(*)> m_RpcMap;
	void Init();
	

private:
	void RPC_Disconnect(class Client* client, NetworkMessage data);
	void RPC_HandShake(class Client* client, NetworkMessage data);
};

