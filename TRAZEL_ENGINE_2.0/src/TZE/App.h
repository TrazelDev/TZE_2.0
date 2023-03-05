#pragma once
#include "Core.h"
#include "Layer/window/Window.h"

#define WINDOW_LAYER_INDEX 0

namespace tze
{
	class TZE_API App
	{
	public:

		App();
		virtual ~App();

		void run();
	private:
		std::vector<Layer*> _layers;
	};

	// this will be defined in the client 
	App* createApp();


}

