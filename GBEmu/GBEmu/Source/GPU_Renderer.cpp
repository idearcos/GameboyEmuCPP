#include "GPU_Renderer.h"
#include <stdexcept>
#include <string>

Renderer::Renderer(GLFWwindow* &window, size_t screen_width, size_t screen_height) :
	window_(window),
	framebuffer_(screen_width * screen_height, 0),
	screen_width_(screen_width),
	screen_height_(screen_height)
{
	glfwSetErrorCallback(GlfwErrorCallback);

	if (!glfwInit())
	{
		throw std::runtime_error("Failed to init GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(screen_width_, screen_height_, "GameBoy :)", NULL, NULL);
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

void Renderer::RenderPixel(size_t x, size_t y, uint8_t color)
{
	framebuffer_[(screen_height_ - 1 - y) * screen_width_ + x] = color;
}

void Renderer::RefreshScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(160, 144, GL_LUMINANCE, GL_UNSIGNED_BYTE, framebuffer_.data());

	glfwSwapBuffers(window_);

	glfwPollEvents();
}
