#pragma once

#include <iostream>
#include <map>
#include "NetworkMessage.h"
#include "Connection.h"

class RPC
{
public:
	RPC() { Init(); }
	void Invoke(const PacketType& type, Connection* client, const NetworkMessage& data);

private:
	typedef std::function<void(Connection*, const NetworkMessage&)> Func;
	std::map<PacketType, Func> m_RpcMap;

private:
	void Init();
	void RegisterRPC(const PacketType& type, void (*func)(Connection*, const NetworkMessage));

};

