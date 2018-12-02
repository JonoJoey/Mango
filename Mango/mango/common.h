#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <deque>
#include <vector>
#include <algorithm>

#include "misc/utility.h"
#include "misc/maths.h"
#include "misc/misc.h"

// glew
#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment(lib, "glew32s.lib")

// glfw3
#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

// opengl
#pragma comment(lib, "opengl32.lib")

// glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// imgui
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

// stb
#include <stb_image.h>

#include <PerlinNoise.hpp>


#define DBG_LOG(x, ...) printf_s("[line:%i - func:%s()] " x "\n", __LINE__, __func__, __VA_ARGS__)
#define DBG_ERROR(x, ...) printf_s("[ERROR][line:%i - func:%s()] " x "\n", __LINE__, __func__, __VA_ARGS__)

#define ASSERT(x) assert(x)



namespace glm
{
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> safe_normalize(vec<L, T, Q> const& x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");

		return length(x) > T(0) ? detail::compute_normalize<L, T, Q, detail::is_aligned<Q>::value>::call(x) : vec<L, T, Q>(T(0));
	}
}



inline size_t GetElementSize(unsigned int element)
{
	switch (element)
	{
		case GL_BYTE: return sizeof(char);
		case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
		case GL_SHORT: return sizeof(short);
		case GL_UNSIGNED_SHORT: return sizeof(unsigned short);
		case GL_INT: return sizeof(int);
		case GL_UNSIGNED_INT: return sizeof(unsigned int);
		case GL_FLOAT: return sizeof(float);
		case GL_2_BYTES: return 2;
		case GL_3_BYTES: return 3;
		case GL_4_BYTES: return 4;
		case GL_DOUBLE: return sizeof(double);
	}

	DBG_ERROR("Element type not supported - 0x%X", element);
	ASSERT(false);
	return 0;
}