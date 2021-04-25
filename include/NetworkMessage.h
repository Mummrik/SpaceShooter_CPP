#pragma once

#include <vector>

enum class PacketType
{
	Disconnect,
	HandShake,
	Ping,
	NewPlayer,
	RemovePlayer,
	Movement,
	RotatePlayer,
	AnimateJetEngine,
	FireBullet,
	MoveBullet,
	RemoveBullet,
	MAX_LENGTH // NOTE: Keep this as last element
};

struct NetworkMessage
{
	PacketType Header;
	bool BodyContainsHeader = false;
	std::vector<char> Body;
	int ReadPos = 0;

	// Construct NetworkMessage of a given type
	NetworkMessage(const PacketType& type) : Header(type) {}

	// Construct NetworkMessage from received data
	NetworkMessage(const std::vector<char>& data)
	{
		for (auto byte : data)
			Body.push_back(byte);

		BodyContainsHeader = true;
		Header = GetPacketType();
		ReadPos = 2;
	}

	size_t GetSize()
	{
		return Body.size();
	}

	PacketType GetPacketType()
	{
		if (BodyContainsHeader == false)
			return Header;

		return (PacketType)((Body[0] & 0xFF) + (Body[1] << 8));
	}

	void PrepareSend()
	{
		if (BodyContainsHeader == false)
		{
			Body.insert(Body.begin(), (char)((uint16_t)Header >> 8));
			Body.insert(Body.begin(), (char)((uint16_t)Header & 0xFF));
			BodyContainsHeader = true;
		}
	}

	void Write(char value)
	{
		Body.push_back(value);
		ReadPos++;
	}

	void Write(const std::vector<char>& values)
	{
		for (auto byte : values)
		{
			Write(byte);
		}
	}

	void Write(int8_t value)
	{
		Write((char)value);
	}

	void Write(uint8_t value)
	{
		Write((char)value);
	}

	void Write(int16_t value)
	{
		Write((char)(value & 0xFF));
		Write((char)(value >> 8));
	}

	void Write(uint16_t value)
	{
		Write((char)(value & 0xFF));
		Write((char)(value >> 8));
	}

	void Write(int32_t value)
	{
		Write((char)(value & 0xFF));
		for (auto i = 1; i < 4; i++)
			Write((char)(value >> (8 * i)));
	}

	void Write(uint32_t value)
	{
		Write((char)(value & 0xFF));
		for (auto i = 1; i < 4; i++)
			Write((char)(value >> (8 * i)));
	}

	void Write(int64_t value)
	{
		Write((char)(value & 0xFF));
		for (auto i = 1; i < 8; i++)
			Write((char)(value >> (8 * i)));
	}

	void Write(uint64_t value)
	{
		Write((char)(value & 0xFF));
		for (auto i = 1; i < 8; i++)
			Write((char)(value >> (8 * i)));
	}

	void Write(float value)
	{
		// TODO: Research how this works
		char result[sizeof(float)];
		memcpy(result, &value, sizeof(float));

		for (auto i = 0; i < sizeof(float); i++)
		{
			Write(result[i]);
		}
	}

	char ReadChar()
	{
		return Body[ReadPos++];
	}

	uint8_t ReadUint8()
	{
		return (uint8_t)ReadChar();
	}

	int8_t ReadInt8()
	{
		return (int8_t)ReadChar();
	}

	uint16_t ReadUint16()
	{
		return (uint16_t)((ReadUint8() & 0xFF) + (ReadUint8() << 8));
	}

	int16_t ReadInt16()
	{
		return (int16_t)((ReadUint8()) + ((int16_t)ReadUint8() << 8));
	}

	uint32_t ReadUint32()
	{
		uint32_t value = ReadUint8() & 0xFF;
		for (auto i = 1; i < 4; i++)
			value += ReadUint8() << (8 * i);

		return value;
	}

	int32_t ReadInt32()
	{
		int32_t value = ReadUint8();
		for (auto i = 1; i < 4; i++)
			value += (int32_t)ReadUint8() << (8 * i);

		return value;
	}

	uint64_t ReadUint64()
	{
		uint64_t value = ReadUint8() & 0xFF;
		for (auto i = 1; i < 8; i++)
			value += ReadUint8() << (8 * i);

		return value;
	}

	int64_t ReadInt64()
	{
		int64_t value = ReadUint8();
		for (auto i = 1; i < 8; i++)
			value += (int64_t)ReadUint8() << (8 * i);

		return value;
	}

	float ReadFloat()
	{
		// TODO: Research this, to either use the bit shift method as int values or change int val
		unsigned char buffer[4];
		for (auto i = 0; i < sizeof(float); i++)
		{
			buffer[i] = ReadUint8();
		}
		return (*(float*)buffer);
	}
};