#include "pch.h"
#include "app.h"

namespace tze {
	App::App()
	{
		_layers.push_back(new Window);
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