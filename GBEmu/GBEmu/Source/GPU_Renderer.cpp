#include "GPU_Renderer.h"
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>

Renderer::Renderer(GLFWwindow* &window, size_t screen_width, size_t screen_height, Zoom zoom) :
	window_(window),
	original_screen_width_{ screen_width },
	original_screen_height_{ screen_height },
	screen_side_zoom_(static_cast<size_t>(zoom)),
	zoomed_screen_width_(screen_width * screen_side_zoom_),
	zoomed_screen_height_(screen_height * screen_side_zoom_),
	framebuffer_(original_screen_width_ * original_screen_height_, Color::Transparent),
	converted_framebuffer_(zoomed_screen_width_ * zoomed_screen_height_, 0)
{
	glfwSetErrorCallback(GlfwErrorCallback);

	if (!glfwInit())
	{
		throw std::runtime_error("Failed to init GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(static_cast<int>(zoomed_screen_width_), static_cast<int>(zoomed_screen_height_), "iniGameBoy :)", NULL, NULL);
	if (nullptr == window_)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to open GLFW window. If you have an Intel GPU, it is not 3.3 compatible.");
	}
	glfwMakeContextCurrent(window_);

	glfwSwapInterval(1);

	int width, height;
	glfwGetFramebufferSize(window_, &width, &height);
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT);
}

Renderer::~Renderer()
{
	glfwDestroyWindow(window_);
	glfwTerminate();
}

void Renderer::GlfwErrorCallback(int error, const char* description)
{
	std::string strError = "Error #" + std::to_string(error) + " from GLFW : " + description;
	throw std::runtime_error(strError);
}

uint8_t Renderer::ConvertColor(Color color)
{
	switch (color)
	{
	case Color::Transparent:
		return 255;
	case Color::LightGrey:
		return 192;
	case Color::DarkGrey:
		return 96;
	case Color::Black:
		return 0;
	default:
		std::stringstream msg;
		msg << "Trying to convert invalid color code: " << static_cast<size_t>(color);
		throw std::runtime_error(msg.str());
	}
}

void Renderer::RenderPixel(size_t x, size_t y, Color color)
{
	framebuffer_[((original_screen_height_ - 1 - y) * original_screen_width_) + x] = color;
}

Color Renderer::GetPixel(size_t x, size_t y) const
{
	return framebuffer_[((original_screen_height_ - 1 - y) * original_screen_width_) + x];
}

void Renderer::RefreshScreen()
{
	for (auto y = 0; y < original_screen_height_; y++)
	{
		for (auto x = 0; x < original_screen_width_; x++)
		{
			// Iterate vertically for a number of lines according to zoom
			for (auto j = 0; j < screen_side_zoom_; j++)
			{
				// Iterate horizontally for a number of pixels in each line according to zoom
				for (auto i = 0; i < screen_side_zoom_; i++)
				{
					converted_framebuffer_[(((y * screen_side_zoom_) + j) * zoomed_screen_width_) + (x * screen_side_zoom_) + i]
						= ConvertColor(framebuffer_[y * original_screen_width_ + x]);
				}
			}
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(zoomed_screen_width_, zoomed_screen_height_, GL_LUMINANCE, GL_UNSIGNED_BYTE, converted_framebuffer_.data());

	/*if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_after_last_buffer_swap_).count() > 16)
	{
		std::cout << "Frame skipped" << std::endl;
	}*/

	glfwSwapBuffers(window_);

	time_after_last_buffer_swap_ = std::chrono::high_resolution_clock::now();

	glfwPollEvents();
}
