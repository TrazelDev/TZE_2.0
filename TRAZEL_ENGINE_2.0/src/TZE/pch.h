#pragma once

// basic c++ libraries:
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <queue>
#include <bitset>
#include <optional>
#include <set>
#include <fstream>

// Logging libraries:
#include <spdlog.h>
#include <sinks/stdout_color_sinks.h>
#include <logger.h>
#include "logging/Log.h"

// window libraries:
#define GLFW_INCLUDE_VULKAN
#define VK_VERSION_1_0
#include <windows.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// Vulkan includes:
#include <vulkan/vulkan.hpp>

// glm includes:
#define GLM_FORCE_RADIANTS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


// A way to make only debugging features:
#ifndef CLIENT_MODE
	#define DEBUG_CMD(x) x
#else
	#define DEBUG_CMD(x)
#endif

// A way for me to tell for things that are plain data and I have the option to toggle here if I want to see it or not:
#ifdef WANT_TO_SHOW_DATA
#pragma region SHOW_DATA macro
#define SHOW_DATA(x) x
#else
#define SHOW_DATA(x)
#endif

// your macro's code goes here

#ifdef WANT_TO_SHOW_DATA
#pragma endregion
#endif