#pragma once

namespace esc {

	enum class TileType : uint8
	{
		None = 0,
		Middle,

		TopSide,
		BottomSide,
		LeftSide,
		RightSide,

		TopLeftCorner,
		TopRightCorner,
		BottomLeftCorner,
		BottomRightCorner
	};

	struct ChunkSpecification
	{
		std::unordered_map<TileType, std::string> Mapping;
	};

#if 0
	class Chunk : public ReferenceCounted
	{
	public:
		Chunk(const ChunkCreateInfo& createInfo);
		virtual ~Chunk();
	};
#endif

}