#pragma once
#include "Core.h"

namespace tze
{
	class TZE_API App
	{
	public:

		App();
		virtual ~App();

		void run();
	};

	// this will be defined in the client 
	App* createApp();


}

