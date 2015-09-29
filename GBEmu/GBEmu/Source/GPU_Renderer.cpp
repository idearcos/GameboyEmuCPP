#include "GPU_Renderer.h"
#include <stdexcept>
#include <string>

Renderer::Renderer()
{
	glfwSetErrorCallback(GlfwErrorCallback);

	if (!glfwInit())
	{
		throw std::runtime_error("Failed to init GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(160, 144, "GameBoy :)", NULL, NULL);
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

void Renderer::RefreshScreen(std::vector<uint8_t> &framebuffer)
{
	//glClear(GL_COLOR_BUFFER_BIT);

	static uint8_t s_intensity = 0;
	uint8_t intensity = s_intensity;
	uint8_t component = 0;

	framebuffer.clear();
	for (auto y = 0; y < 144; y++)
	{
		for (auto x = 0; x < 160; x++)
		{
			if (x < 53)
			{
				component = 0;
			}
			else if (x >= 53 && x < 106)
			{
				component = 1;
			}
			else if (x >= 106)
			{
				component = 2;
			}

			framebuffer[160 * 3 * y + 3 * x + component] = intensity;
		}

		if (++intensity == 160)
		{
			intensity = 0;
		}
	}
	if (++s_intensity == 160)
	{
		s_intensity = 0;
	}

	glDrawPixels(160, 144, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());

	glfwSwapBuffers(window_);

	glfwPollEvents();
}
