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

		void SetUniformF2(int location, glm::fvec2 v) const;
		void SetUniformF2(const std::string& name, glm::fvec2 v) { SetUniformF2(GetUniformLoc(name), v); }

		void SetUniformF3(int location, glm::fvec3 v) const;
		void SetUniformF3(const std::string& name, glm::fvec3 v) { SetUniformF3(GetUniformLoc(name), v); }

		void SetUniformF4(int location, glm::fvec4 v) const;
		void SetUniformF4(const std::string& name, glm::fvec4 v) { SetUniformF4(GetUniformLoc(name), v); }

		void SetUniformI1(int location, int v) const;
		void SetUniformI1(const std::string& name, int v) { SetUniformI1(GetUniformLoc(name), v); }

		void SetUniformMat3(int location, const glm::mat3& mat) const;
		void SetUniformMat3(const std::string& name, const glm::mat3& mat) { SetUniformMat3(GetUniformLoc(name), mat); }

		void SetUniformMat4(int location, const glm::mat4& mat) const;
		void SetUniformMat4(const std::string& name, const glm::mat4& mat) { SetUniformMat4(GetUniformLoc(name), mat); }

	private:
		unsigned int m_program = 0xFFFFFFFF;
		std::unordered_map<std::string, int> m_cached_locations;
	};
} // namespace Mango