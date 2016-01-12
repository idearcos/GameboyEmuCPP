#pragma once

#include <vector>
#include <cstdint>
#include <chrono>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "GPU_Color.h"

class Renderer
{
public:
	enum class Scale
	{
		x1 = 1,
		x4 = 2,
		x9 = 3,
		x16 = 4,
		x25 = 5
	};

	Renderer(GLFWwindow* &window, size_t screen_width, size_t screen_height, Scale scale);
	~Renderer();

	static void GlfwErrorCallback(int error, const char* description);

	void RenderPixel(size_t x, size_t y, Color color);
	Color GetPixel(size_t x, size_t y) const;
	void RefreshScreen();

private:
	uint8_t ConvertColor(Color color);

private:
	GLFWwindow*& window_;

	const size_t screen_width_;
	const size_t screen_height_;

	std::vector<Color> framebuffer_;
	std::vector<uint8_t> converted_framebuffer_;

	std::chrono::high_resolution_clock::time_point time_after_last_buffer_swap_;

private:
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;
};
