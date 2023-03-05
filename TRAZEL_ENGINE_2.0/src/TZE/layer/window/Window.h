#pragma once
#include "pch.h"
#include "layer/Layer.h"


namespace tze
{
	class Window : public Layer
	{
	public:
		Window(int width = 1280, int height = 720, const char* title = "TRAZEL_ENGINE");
		~Window();

		/// <summary>
		/// A function that the layer level binds the class to have because the engine contains layer that are going to be run one after another
		/// </summary>
		void run() override;

		/// <summary>
		/// a getter that gives the application level a way to know if the user closed the window
		/// </summary>
		/// <returns>pointer to the class variable that says if the window has been closed or not</returns>
		bool* closeWindowFlag();
	private:
		void buildGLFWWindow();

		GLFWwindow* _window;

		// properties of the window:
		std::string _title;
		uint32_t _width;
		uint32_t _height;

		// this is a special variable that lets me signal to the application that the user has closed the window:
		bool _closedWindowFLag; 
	};

}

