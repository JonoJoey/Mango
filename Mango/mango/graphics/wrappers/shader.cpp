#include "shader.h"

#include <fstream>


namespace Mango
{
	bool Shader::Setup(const std::string& vertex_src, const std::string& fragment_src)
	{
		ASSERT(!vertex_src.empty());
		ASSERT(!fragment_src.empty());

		const auto CompileShader = [](const std::string& src, unsigned int type) -> unsigned int
		{
			const auto shader = glCreateShader(type);
			const char* source = src.c_str();

			int size = int(src.size());
			glShaderSource(shader, 1, &source, &size);
			glCompileShader(shader);

			int status; glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			if (status != GL_FALSE)
				return shader;

			char error[256];
			glGetShaderInfoLog(shader, 256, nullptr, error);
			DBG_ERROR("Error compiling shader - type:0x%X - %s", type, error);
			return 0xFFFFFFFF;
		};

		const auto vertex_shader = CompileShader(vertex_src, GL_VERTEX_SHADER);
		if (vertex_shader == 0xFFFFFFFF)
			return false;

		const auto fragment_shader = CompileShader(fragment_src, GL_FRAGMENT_SHADER);
		if (fragment_shader == 0xFFFFFFFF)
			return false;

		m_program = glCreateProgram();
		glAttachShader(m_program, vertex_shader);
		glAttachShader(m_program, fragment_shader);
		glLinkProgram(m_program);
		glValidateProgram(m_program);

		int status; glGetProgramiv(m_program, GL_VALIDATE_STATUS, &status);
		if (status != GL_TRUE)
		{
			DBG_ERROR("Failed to validate program");
			return false;
		}

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return true;
	}
	void Shader::Release()
	{
		if (!IsUtilRegistered())
			return;
		UnregisterUtil();

		if (m_program == 0xFFFFFFFF)
			return;

		glDeleteProgram(m_program);
		m_program = 0xFFFFFFFF;
	}

	void Shader::Bind() const
	{
		glUseProgram(m_program);
	}
	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	int Shader::GetUniformLoc(const std::string& name)
	{
		if (auto pair = m_cached_locations.find(name); pair != m_cached_locations.end())
			return pair->second;

		const int location = glGetUniformLocation(m_program, name.c_str());
		if (location == -1)
			DBG_ERROR("Failed to get uniform location - [%s]", name.c_str());

		m_cached_locations[name] = location;
		return location;
	}

	void Shader::SetUniformF1(int location, float v) const
	{
		glUniform1fv(location, 1, &v);
	}
	void Shader::SetUniformF2(int location, glm::fvec2 v) const
	{
		glUniform2fv(location, 1, &v[0]);
	}
	void Shader::SetUniformF3(int location, glm::fvec3 v) const
	{
		glUniform3fv(location, 1, &v[0]);
	}
	void Shader::SetUniformF4(int location, glm::fvec4 v) const
	{
		glUniform4fv(location, 1, &v[0]);
	}

	void Shader::SetUniformI1(int location, int v) const
	{
		glUniform1i(location, v);
	}

	void Shader::SetUniformMat3(int location, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
	}
	void Shader::SetUniformMat4(int location, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
	}

	std::string Shader::ReadFile(const std::string& file_path)
	{
		std::ifstream file(file_path);
		if (!file)
		{
			DBG_ERROR("Failed to open file - [%s]", file_path.c_str());
			return "";
		}

		file.seekg(0, std::ios::end);

		std::string contents;
		contents.resize(size_t(file.tellg()));

		file.seekg(0, std::ios::beg);
		file.read(&contents[0], contents.size());

		file.close();
		return contents;
	}
} // namespace Mango