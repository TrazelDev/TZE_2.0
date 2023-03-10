#pragma once
#include "layers/Layer.h"
#include "vk_logging/vkLogging.h"

namespace tze
{
	
	class Pipeline : public Layer
	{
	public:
		Pipeline();
		~Pipeline();




		void run() override;
	private:
	};

}
