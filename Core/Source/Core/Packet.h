#pragma once

#include "PrimitiveTypes.h"

namespace esc {

	enum class PacketType : uint32
	{
		None = 0,
		Connect = 1,
		Disconnect = 2,
		ID = 3,
		Input = 4,
		LevelData = 5,
		TransformUpdate = 6,
		PlayerUpdate = 7,
		PhysicsData = 8,
		EntityUpdate = 9
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

	struct InputPacket
	{
		float HorizontalAxis = 0.0f;
		float VerticalAxis = 0.0f;
		float VelocityX = 0.0f;
		float VelocityY = 0.0f;
		float Speed = 0.0f;
		bool Jump = false;

		bool operator==(const InputPacket& other) const
		{
			return HorizontalAxis == other.HorizontalAxis && 
				VerticalAxis == other.VerticalAxis &&
				VelocityX == other.VelocityX &&
				VelocityY == other.VelocityY &&
				Speed == other.Speed && 
				Jump == other.Jump;
		}
	};

	struct LevelDataPacket
	{
		
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
			return PositionX == other.PositionX && 
				PositionY == other.PositionY && 
				Angle == other.Angle && 
				ScaleX == other.ScaleX && 
				ScaleY == other.ScaleY;
		}
	};

	struct PlayerUpdatePacket
	{
		char TextureName[128] = { '\n' };
		float ColorR = 0.0f;
		float ColorG = 0.0f;
		float ColorB = 0.0f;

		PlayerUpdatePacket() = default;

		bool operator==(const PlayerUpdatePacket& other) const
		{
			return strcmp(TextureName, other.TextureName) == 0 &&
				ColorR == other.ColorR && 
				ColorG == other.ColorG && 
				ColorB == other.ColorB;
		}
	};

	struct PhysicsDataPacket
	{
		uint32 NumPhysicsEntities = 0;

		bool operator==(const PhysicsDataPacket& other) const
		{
			return NumPhysicsEntities == other.NumPhysicsEntities;
		}
	};

	struct EntityUpdatePacket
	{
		uint32 ID = 0;
		float PositionX = 0.0f;
		float PositionY = 0.0f;
		float Angle = 0.0f;
		float ScaleX = 0.0f;
		float ScaleY = 0.0f;
		float VelocityX = 0.0f;
		float VelocityY = 0.0f;

		bool operator==(const EntityUpdatePacket& other) const
		{
			return ID == other.ID && 
				PositionX == other.PositionX &&
				PositionY == other.PositionY &&
				Angle == other.Angle &&
				ScaleX == other.ScaleX &&
				ScaleY == other.ScaleY &&
				VelocityX == other.VelocityX &&
				VelocityY == other.VelocityY;
		}
	};

}