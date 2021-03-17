#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>

using asio::ip::udp;

enum PacketType : uint16_t
{
	Disconnect,
	HandShake,
	Ping,
	NewPlayer,
	Length,
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

	void Send(udp::socket* Socket, const udp::endpoint& Receiver)
	{
		PrepareSend();
		Socket->send_to(asio::buffer(Body), Receiver);
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
			Body.insert(Body.begin(), (char)(Header >> 8));
			Body.insert(Body.begin(), (char)(Header & 0xFF));
			BodyContainsHeader = true;
		}
	}

	void Write(const char& value)
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

	void Write(const int8_t& value)
	{
		Write((char)value);
	}

	void Write(const uint8_t& value)
	{
		Write((char)value);
	}

	void Write(const int16_t& value)
	{
		Write((char)(value & 0xFF));
		Write((char)(value >> 8));
	}

	void Write(const uint16_t& value)
	{
		Write((char)(value & 0xFF));
		Write((char)(value >> 8));
	}

	void Write(const int32_t& value)
	{
		Write((char)(value & 0xFF));
		for (auto i = 1; i < 4; i++)
			Write((char)(value >> (8 * i)));
	}

	void Write(const uint32_t& value)
	{
		Write((char)(value & 0xFF));
		for (auto i = 1; i < 4; i++)
			Write((char)(value >> (8 * i)));
	}

	void Write(const int64_t& value)
	{
		Write((char)(value & 0xFF));
		for (auto i = 1; i < 8; i++)
			Write((char)(value >> (8 * i)));
	}

	void Write(const uint64_t& value)
	{
		Write((char)(value & 0xFF));
		for (auto i = 1; i < 8; i++)
			Write((char)(value >> (8 * i)));
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
};