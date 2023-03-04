#pragma once

#ifdef TZE_PLATFORM_WINDOWS
	#ifndef Client_MODE
	extern tze::App* tze::createApp();
	int main(int argc, char** argv)
	{
		tze::App* App = tze::createApp();
		App->run();
		delete App;
	}
	#else	
	extern tze::app* tze::createApp();
	int WinMain(HINSTANCE inst, HINSTANCE prev_inst, LPSTR arg, int show_cmd)
	{
		tze::app* App = tze::createApp();
		App->run();
		delete App;
	
		return 0;
	}
	#endif
#endif