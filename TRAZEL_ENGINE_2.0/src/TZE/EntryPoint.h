#pragma once
#include "logging/Log.h"

#ifdef TZE_PLATFORM_WINDOWS
	#ifndef CLIENT_MODE
	extern tze::App* tze::createApp();
	int main(int argc, char** argv)
	{
		tze::Log::init();

		tze::App* App = tze::createApp();
		App->run();
		delete App;
	}
	#else	
extern tze::App* tze::createApp();
	int WinMain(HINSTANCE inst, HINSTANCE prev_inst, LPSTR arg, int show_cmd)
	{
		tze::App* App = tze::createApp();
		App->run();
		delete App;
	}
	#endif
#endif