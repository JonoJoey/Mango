#pragma once

#include "../../common.h"

#include <unordered_map>


namespace Mango
{
	class Shader : private Utility
	{
	public:
		Shader() = default;
		Shader(const std::string& vertex_src, const std::string& fragment_src) { ASSERT(Setup(vertex_src, fragment_src)); }
		~Shader() { Release(); }

		bool Setup(const std::string& vertex_src, const std::string& fragment_src);
		void Release() override;

		void Bind() const;
		static void Unbind();

		static std::string ReadFile(const std::string& file_path);

		int GetUniformLoc(const std::string& name);

		static void SetUniformF1(int location, float v);
		static void SetUniformF2(int location, float v1, float v2);
		static void SetUniformF3(int location, float v1, float v2, float v3);
		static void SetUniformF4(int location, float v1, float v2, float v3, float v4);
		static void SetUniformMat4(int location, const glm::mat4& mat);

	private:
		unsigned int m_program = 0xFFFFFFFF;
		std::unordered_map<std::string, int> m_cached_locations;
	};
} // namespace Mango