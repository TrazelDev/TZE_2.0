#include "pch.h"
#include "Window.h"

tze::Window::Window(int width, int height, const char* title)
{
	_closedWindowFLag = false;
	_width = width;
	_height = height;
	_title.assign(title);
	_window = nullptr;

	buildGLFWWindow();
	TZE_ENGINE_INFO("Successfully finished creating the Window layer");
}

tze::Window::~Window()
{
	glfwTerminate();
}

void tze::Window::run()
{
	glfwPollEvents();
	_closedWindowFLag = glfwWindowShouldClose(_window);
}

bool* tze::Window::closeWindowFlag()
{
	return &_closedWindowFLag;
}

GLFWwindow* tze::Window::getWindow() const
{
	return _window;
}

uint32_t tze::Window::getWidth() const
{
	return _width;
}

uint32_t tze::Window::getHeight() const
{
	return _height;
}

void tze::Window::buildGLFWWindow()
{
	// initializing GLFW
	if (!glfwInit())
	{
		TZE_ENGINE_ERR("Error initializing GLFW");
	}


	// no default rendering client VULKAN will be hooked up later on 
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// resizing breaks the swapChain so it will be disabled for now
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);  
	if (!_window)
	{
		TZE_ENGINE_ERR("Failed to create the GLFW window variable");
	}
}	