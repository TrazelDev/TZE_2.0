#include <iostream>
#include "TZE.h"

class ClientApp : public tze::App
{
public:
	ClientApp()
	{
	}
	~ClientApp()
	{
	}
};


tze::App* tze::createApp()
{
	return new ClientApp;
}