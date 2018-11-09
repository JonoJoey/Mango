#include "file_parser.h"

#include <fstream>

#include "../common.h"


namespace Mango
{
	bool LoadWavefront(const std::string& file_path, WavefrontFormat& format)
	{
		std::ifstream file(file_path);
		if (!file)
		{
			DBG_ERROR("Failed to open file: \"%s\"", file_path.c_str());
			return false;
		}

		std::deque<std::array<float, 3>> positions;
		std::deque<std::array<float, 3>> normals;
		std::deque<std::array<float, 2>> tex_coords;

		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty() || line[0] == '#')
				continue;

			std::deque<std::string> tokens({""});
			for (const char c : line)
			{
				if (c == ' ')
					tokens.emplace_back();
				else
					tokens.back() += c;
			}
			
			if (tokens.size() == 4)
			{
				if (tokens[0] == "v") // vertex
				{

				}
				else if (tokens[0] == "vn") // normal
				{

				}
				else if (tokens[0] == "f") // face
				{

				}
			}
			else if (tokens.size() == 3)
			{
				if (tokens[0] == "vt") // texture coords
				{

				}
			}
		}

		//format.m_positions.clear(); 
		//format.m_normals.clear();
		//format.m_tex_coords.clear();
		//format.m_indices.clear();

		file.close();
		return true;
	}
} // namespace Mango