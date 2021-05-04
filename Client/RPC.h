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
	void RemovePlayer(Connection* client, NetworkMessage& data);
	void Movement(Connection* client, NetworkMessage& data);
	void Rotation(Connection* client, NetworkMessage& data);
	void AnimateJetEngine(Connection* client, NetworkMessage& data);
	void FireBullet(Connection* client, NetworkMessage& data);
	void MoveBullet(Connection* client, NetworkMessage& data);
	void RemoveBullet(Connection* client, NetworkMessage& data);
	void Ping(Connection* client, NetworkMessage& data);
	void AnimateDamage(Connection* client, NetworkMessage& data);
	void KDStats(Connection* client, NetworkMessage& data);
	void UpdateHealth(Connection* client, NetworkMessage& data);

};

