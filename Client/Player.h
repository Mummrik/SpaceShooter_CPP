#pragma once
#include "Game.h"

class Player
{
public:
	Player(uint64_t uid, uint8_t spriteId, olc::vf2d position, float rotation) : m_Uid(uid), m_SpriteId(spriteId), m_Position(position), m_Rotation(rotation) {}

private:
	uint64_t m_Uid;
	olc::vf2d m_Position;
	float m_Rotation = 0;
	uint8_t m_SpriteId;
	olc::Pixel m_Tint = olc::WHITE;
	float m_AnimateTimer = 0;
	float m_MaxHealth = 100;
	float m_Health = m_MaxHealth;

public:
	bool IsActive = false;
	bool IsThrusting = false;
	void OnUpdate(Game* game, float fElapsedTime);
	uint64_t GetPlayerUid();
	olc::vf2d GetPlayerPosition();
	uint8_t GetPlayerSpriteId();
	float GetPlayerRotation();
	void SetRotation(float rotation);
	void SetPosition(const olc::vf2d& position);
	void SetTint(const olc::Pixel& tint, float duration = 1.0f);
	void SetHealth(float health);

private:
	void Render(Game* game, float fElapsedTime);
	void DrawHealthBar(Game* game);
};