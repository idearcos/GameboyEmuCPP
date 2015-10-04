#pragma once

#include <vector>
#include <cstdint>
#include "GLFW/glfw3.h"

class Renderer
{
public:
	Renderer(GLFWwindow* &window, size_t screen_width, size_t screen_height);
	~Renderer();

	static void GlfwErrorCallback(int error, const char* description);

	void RenderPixel(size_t x, size_t y, uint8_t color);
	void RefreshScreen();

private:
	GLFWwindow*& window_;
	std::vector<uint8_t> framebuffer_;

	const size_t screen_width_;
	const size_t screen_height_;

private:
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;
};
