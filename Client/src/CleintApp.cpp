#include <iostream>
#include <vector>
#include "TZE.h"


class ClientApp : public tze::App
{
public:
	ClientApp() : App()
	{
		
	}
	~ClientApp()
	{

	}
};


tze::App* tze::createApp()
{
	ClientApp* clientApp = new ClientApp(); // creating the application

	// constructing the game object:
	tze::GameObject* triangle = tze::GameObject::p_createGameObj();
	std::vector<tze::Model::Vertex> vertices // deciding what will be the location of the triangle on the screen:
	{
		{{0.5f, -0.5f }},
		{{0.0f, 0.5f  }},
		{{-0.5f, -0.5f}}
	};
	std::shared_ptr<tze::Model> model = std::make_shared<tze::Model>(clientApp->getPhysicalDevice(), clientApp->getLogicalDevice(), vertices);
	triangle->model = model;
	triangle->color = { 0.8f, 0.0f, 0.0f };
	triangle->transform_2D = tze::Transform_2DComponent();
	clientApp->addLayer(triangle);

	return clientApp;
}