#pragma once

#include "PrimitiveTypes.h"

namespace esc {

	enum class PacketType : uint32
	{
		None = 0,
		Username = 2,
		ID = 3,
		TransformUpdate = 4
	};
	
	struct PacketHeader
	{
		int32 ID = 0;
		PacketType Type = PacketType::None;
	};

	struct IDPacket
	{
		uint32 ID = 0;
	};

	struct UsernamePacket
	{
		char Username[64] = { '\n' };
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

}