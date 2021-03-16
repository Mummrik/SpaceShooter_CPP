#pragma once

#include <iostream>
#include <vector>
#include "NetworkMessage.h"
#include "Connection.h"

class RPC
{
public:
	RPC()
	{
		Init();
	}


	void Invoke(PacketType type, Connection* client, NetworkMessage data)
	{
		if ((size_t)type < 0 && (size_t)type > m_RpcVector.size())
			return;

		((void (*)(Connection*, NetworkMessage)) m_RpcVector[type])(client, data);
	}

private:
	void Init();
	std::vector<void* (*)(void*)> m_RpcVector;

};

