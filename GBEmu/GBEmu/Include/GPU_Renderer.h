#pragma once

#include <vector>
#include <cstdint>
#include "glfw3.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	static void GlfwErrorCallback(int error, const char* description);

	void RefreshScreen(const std::vector<uint8_t> &frameBuffer);

private:
	GLFWwindow* window_;
};
