#pragma once

#include <iostream>
#include <thread>
#include "RPC.h"


#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>

class Client : public olc::PixelGameEngine
{
public:
	Client(std::string host, std::string port) : m_Socket(m_Context)
	{
		sAppName = "Space Wars";
		udp::resolver resolver(m_Context);
		m_RemoteEndpoint = *resolver.resolve(udp::v4(), host, port).begin();
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		if (Connect(m_RemoteEndpoint))
		{
			m_Listener = std::thread(&Client::OnListen, this);
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame

		return true;
	}

	bool OnUserDestroy() override
	{
		// called when application close
		NetworkMessage msg(PacketType::Disconnect);
		msg.Send(&m_Socket, m_RemoteEndpoint);

		// Should be set by RPC_Disconnect
		//IsConnected = false;

		return false;
	}

private:
	asio::io_context m_Context;
	std::thread m_Listener;
	udp::socket m_Socket;
	udp::endpoint m_RemoteEndpoint;
	RPC m_Rpc;

private:
	bool Connect(udp::endpoint RemoteEndpoint);
	void OnHandle(std::vector<char> Data);
	void OnListen();

public:
	uint16_t ConnectionId = 0;
	bool IsConnected = false;
	udp::socket* GetSocket() { return &m_Socket; }
	udp::endpoint GetRemoteEndpoint() { return m_RemoteEndpoint; }
	std::thread* GetListenerThread() { return &m_Listener; }

	
};