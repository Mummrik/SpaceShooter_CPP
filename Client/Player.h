#pragma once

#include <string>
#include <NetworkMessage.h>

class Player
{
public:
	Player() {}

private:
	std::string m_Name;
	//olc::vf2d m_Position;

public:
	void Create(const std::string& name);
	void OnUpdate(/*olc::PixelGameEngine* Engine,*/ float fElapsedTime);
	const std::string& GetName() { return m_Name; }
};