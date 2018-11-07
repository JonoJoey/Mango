#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "misc/utility.h"

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


#define DBG_LOG(x, ...) printf_s("[line:%i - func:%s()] " x "\n", __LINE__, __func__, __VA_ARGS__)
#define ASSERT(x) assert(x)