#pragma once

namespace tze
{
	class TZE_API Layer
	{
	public:
		/// <summary>
		/// this is a function that is going called from the application so it binds 
		/// every layer of the application to contains  
		/// </summary>
		virtual void run() = 0;
	private:
	};

}
