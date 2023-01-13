#pragma once

#include "PrimitiveTypes.h"

namespace esc {

	enum class PacketType : uint32
	{
		Username = 2,
		ID = 3,
		TransformUpdate = 4
	};
	
	// TODO: Headers

	struct TransformUpdate
	{
		float PositionX = 0.0f;
		float PositionY = 0.0f;
		float Angle = 0.0f;
		float ScaleX = 0.0f;
		float ScaleY = 0.0f;

		TransformUpdate() = default;

		bool operator==(const TransformUpdate& other) const
		{
			return PositionX == other.PositionX && PositionY == other.PositionY && Angle == other.Angle && ScaleX == other.ScaleX && ScaleY == other.ScaleY;
		}
	};

}