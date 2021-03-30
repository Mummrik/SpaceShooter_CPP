#pragma once

#include <iostream>
#include <array>
#include <NetworkMessage.h>

class Connection;
class Game;

class RPC
{
public:
	RPC() {}
	void Invoke(RPC& rpc, const PacketType& type, Connection* client, NetworkMessage& data);
	void Initialize(Game* game);

private:
	typedef void(RPC::* RpcCallbacks)(Connection*, NetworkMessage&);
	RpcCallbacks m_Rpc[(size_t)PacketType::MAX_LENGTH];
	Game* m_Game;

private:
	void Disconnect(Connection* client, NetworkMessage& data);
	void HandShake(Connection* client, NetworkMessage& data);
	void NewPlayer(Connection* client, NetworkMessage& data);
	void Movement(Connection* client, NetworkMessage& data);
	void RemovePlayer(Connection* client, NetworkMessage& data);

};

