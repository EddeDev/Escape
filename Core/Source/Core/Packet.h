#pragma once

#include "PrimitiveTypes.h"

namespace esc {

	enum class PacketType : uint32
	{
		Username = 2,
		ID = 3,
		TransformUpdate = 4
	};

	struct TransformUpdate
	{
		float PositionX;
		float PositionY;
		float Angle;
		float ScaleX;
		float ScaleY;

		bool operator==(const TransformUpdate& other) const
		{
			return PositionX == other.PositionX && PositionY == other.PositionY && Angle == other.Angle && ScaleX == other.ScaleX && ScaleY == other.ScaleY;
		}
	};



}