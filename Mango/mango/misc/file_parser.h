#pragma once

#include <vector>
#include <array>
#include <string>


namespace Mango
{
	struct WavefrontFormat // wavefront .obj
	{
		std::vector<std::array<float, 3>> m_positions;
		std::vector<std::array<float, 3>> m_normals;
		std::vector<std::array<float, 2>> m_tex_coords;
		std::vector<std::array<unsigned int, 3>> m_indices;
	};

	bool LoadWavefront(const std::string& file_path, WavefrontFormat& format);
} // namespace Mango