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
		std::deque<std::array<float, 2>> tex_coords;
		std::deque<std::array<float, 3>> normals;
		std::deque<std::array<int, 9>> faces;

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
					positions.push_back({ float(atof(tokens[1].c_str())), float(atof(tokens[2].c_str())), float(atof(tokens[3].c_str())) });
				}
				else if (tokens[0] == "vn") // normal
				{
					normals.push_back({ float(atof(tokens[1].c_str())), float(atof(tokens[2].c_str())), float(atof(tokens[3].c_str())) });
				}
				else if (tokens[0] == "f") // face
				{
					size_t num_slashes = 0;
					for (const char c : tokens[1])
					{
						if (c == '/')
							num_slashes++;
					}

					std::array<int, 9> face = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
					switch (num_slashes)
					{
					case 0:
						face = { atoi(tokens[1].c_str()), -1, -1, atoi(tokens[2].c_str()), -1, -1, atoi(tokens[3].c_str()), -1, -1 };
						break;
					case 1:
						for (int i = 0; i < 3; i++)
							ASSERT(sscanf_s(tokens[i + 1].c_str(), "%i/%i", &face[i * 3], &face[i * 3 + 1]) == 2);
						break;
					case 2:
						for (int i = 0; i < 3; i++)
							ASSERT(sscanf_s(tokens[i + 1].c_str(), "%i/%i/%i", &face[i * 3], &face[i * 3 + 1], &face[i * 3 + 2]) == 3);
						break;
					}

					faces.push_back(face);
				}
			}
			else if (tokens.size() == 3)
			{
				if (tokens[0] == "vt") // texture coords
				{
					tex_coords.push_back({ float(atof(tokens[1].c_str())), float(atof(tokens[2].c_str())) });
				}
			}
		}

		file.close();

		// make sure they're all empty
		format.m_positions.clear(); 
		format.m_tex_coords.clear();
		format.m_normals.clear();
		format.m_indices.clear();

		for (const auto& face : faces)
		{
			const size_t size = format.m_indices.size() * 3;
			format.m_indices.push_back({ size, size + 1, size + 2 });

			format.m_positions.push_back(positions[face[0] - 1]);
			format.m_positions.push_back(positions[face[3] - 1]);
			format.m_positions.push_back(positions[face[6] - 1]);

			format.m_tex_coords.push_back(tex_coords[face[1] - 1]);
			format.m_tex_coords.push_back(tex_coords[face[4] - 1]);
			format.m_tex_coords.push_back(tex_coords[face[7] - 1]);

			format.m_normals.push_back(normals[face[2] - 1]);
			format.m_normals.push_back(normals[face[5] - 1]);
			format.m_normals.push_back(normals[face[8] - 1]);
		}

		return true;
	}
} // namespace Mango