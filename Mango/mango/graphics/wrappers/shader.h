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

		void SetUniformF1(int location, float v) const;
		void SetUniformF1(const std::string& name, float v) { SetUniformF1(GetUniformLoc(name), v); }

		void SetUniformF2(int location, float v1, float v2) const;
		void SetUniformF2(const std::string& name, float v1, float v2) { SetUniformF2(GetUniformLoc(name), v1, v2); }

		void SetUniformF3(int location, float v1, float v2, float v3) const;
		void SetUniformF3(const std::string& name, float v1, float v2, float v3) { SetUniformF3(GetUniformLoc(name), v1, v2, v3); }

		void SetUniformF4(int location, float v1, float v2, float v3, float v4) const;
		void SetUniformF4(const std::string& name, float v1, float v2, float v3, float v4) { SetUniformF4(GetUniformLoc(name), v1, v2, v3, v4); }

		void SetUniformMat4(int location, const glm::mat4& mat) const;
		void SetUniformMat4(const std::string& name, const glm::mat4& mat) { SetUniformMat4(GetUniformLoc(name), mat); }

	private:
		unsigned int m_program = 0xFFFFFFFF;
		std::unordered_map<std::string, int> m_cached_locations;
	};
} // namespace Mango