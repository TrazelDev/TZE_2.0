#include <iostream>
#include <vector>
#include "TZE.h"

class ClientApp : public tze::App
{
public:
	ClientApp() : App()
	{
		std::vector<glm::vec3> _colors
		{
			{255, 0, 0},
			{204, 102, 0},
			{255, 255, 0},
			{0, 204, 0},
			{0, 0, 204},
			{102, 0, 104},
			{153, 0, 153},
		};
		
		for (int i = _colors.size(); i > 0; i--)
		{
			std::vector<tze::Model::Vertex> vertices // deciding what will be the location of the triangle on the screen:
			{
				{{i * 0.1f, i * -0.1f }},
				{{i * 0.1f, i * 0.1f  }},
				{{i * -0.1f,i * -0.1f}}
			};
			makeTriangle(vertices, _colors[i - 1]);
		}
	}
	~ClientApp()
	{
	}

	void makeTriangle(const std::vector<tze::Model::Vertex>& vertex, const glm::vec3& color)
	{
		//  constructing the game object:
		tze::GameObject* triangle = tze::GameObject::p_createGameObj();
		std::vector<tze::Model::Vertex> vertices // deciding what will be the location of the triangle on the screen:
		{
			vertex
		};
		
		std::shared_ptr<tze::Model> model = std::make_shared<tze::Model>(getPhysicalDevice(), getLogicalDevice(), vertices);
		triangle->_model = model;
		triangle->_color = color;
		triangle->_transform_2D = tze::Transform_2DComponent();
		triangle->_transform_2D._spin = true;
		_renderer.addGameObjects(triangle);
	}
};

tze::App* tze::createApp()
{
	ClientApp* clientApp = new ClientApp(); // creating the application
	return clientApp;
}