#include "pch.h"
#include "app.h"

#include "Layers/Window/Window.h"
#include "layers/Vulkan/Instance.h"

namespace tze {
	App::App()
	{
		_layers.push_back(new Window());
		_layers.push_back(new Instance(((Window*)_layers[WINDOW_LAYER_INDEX])->getWindow()));
		
	}

	App::~App()
	{

	}


	void App::run()
	{
		bool* closingFlag = ((Window*)_layers[WINDOW_LAYER_INDEX])->closeWindowFlag();

		while (!*closingFlag)
		{
			for (Layer* layer : this->_layers)
			{
				layer->run();
			}

			// majorLay->onUpdate();
			//imguiLay->onUpdate();
		}

		TZE_ENGINE_INFO("Successfully closed the window");
	}
}