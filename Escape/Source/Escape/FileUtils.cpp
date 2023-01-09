#include "EscapePCH.h"
#include "FileUtils.h"

namespace Escape {

	std::string FileUtils::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			std::streampos size = in.tellg();
			in.seekg(0, std::ios::beg);
			result.resize(size);
			in.read(result.data(), size);
		}
		else
		{
			std::cerr << "Could not open file '" << filepath << "'!" << std::endl;
		}
		in.close();
		return result;
	}

}