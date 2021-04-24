#pragma once

#include <iostream>
#include "Connection.h"
#include "RPC.h"
#include <map>
#include <mutex>
#include "Vec2d.h"
#include <queue>

class Player;

class Server
{
public:
	Server(uint16_t port) : m_Socket(m_Context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), m_Rpc(this)
	{
		Start(port);
	}
	~Server() { Shutdown(); }

private:
	void Start(uint16_t port);
	bool Shutdown();
	bool IsRunning;

private:
	asio::io_context m_Context;
	asio::ip::udp::socket m_Socket;
	std::thread m_Listener;
	std::thread m_Game;
	RPC m_Rpc;
	std::vector<Connection*> m_Connections;
	std::array<std::shared_ptr<Player>, 1000> m_Players;
	std::queue<uint64_t> m_RemovePlayer;

public:
	std::mutex mutex;
	const float FixedDeltaTime = (1.0f / 60.0f) * 0.01f;
	uint8_t RandomSpriteId;
	std::queue<uint64_t> SpawnPlayer;

private:
	void OnHandle(const std::vector<char>& Data, const asio::ip::udp::endpoint& RemoteEndpoint);
	void OnListen();
	void GameLoop();
	uint16_t FindEmptyPlayerIndex();
	void CheckRemovePlayer();
	void SpawnNewPlayer();

public:
	void SendToAll(NetworkMessage& msg, bool reliable);
	Connection* GetConnection(const asio::ip::udp::endpoint& RemoteEndpoint);
	std::vector<Connection*> GetConnections();
	void TerminateClient(Connection* client);
	void NewBullet(uint64_t owner, Vec2d velocity);
	size_t GetPlayerIndex(uint64_t uid);
	std::shared_ptr<Player> GetLastPlayerElement();
	std::shared_ptr<Player> GetPlayer(uint64_t uid);
};
