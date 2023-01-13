#pragma once

#include "PrimitiveTypes.h"

namespace esc {

	enum class PacketType : uint32
	{
		None = 0,
		Connect = 1,
		Disconnect = 2,
		ID = 3,
		TransformUpdate = 4,
		ColorUpdate = 5
	};
	
	struct PacketHeader
	{
		int32 ID = 0;
		PacketType Type = PacketType::None;
	};

	struct ConnectPacket
	{
		char Username[64] = { '\n' };
	};

	struct DisconnectPacket
	{
		int32 ID = 0;
	};

	struct IDPacket
	{
		int32 ID = 0;
	};

	struct TransformUpdatePacket
	{
		float PositionX = 0.0f;
		float PositionY = 0.0f;
		float Angle = 0.0f;
		float ScaleX = 0.0f;
		float ScaleY = 0.0f;

		TransformUpdatePacket() = default;

		bool operator==(const TransformUpdatePacket& other) const
		{
			return PositionX == other.PositionX && PositionY == other.PositionY && Angle == other.Angle && ScaleX == other.ScaleX && ScaleY == other.ScaleY;
		}
	};

	struct ColorUpdatePacket
	{
		float ColorR = 0.0f;
		float ColorG = 0.0f;
		float ColorB = 0.0f;

		ColorUpdatePacket() = default;

		bool operator==(const ColorUpdatePacket& other)
		{
			return ColorR == other.ColorR && ColorG == other.ColorG && ColorB == other.ColorB;
		}
	};

}