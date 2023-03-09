#pragma once
#include "layers/Layer.h"

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
