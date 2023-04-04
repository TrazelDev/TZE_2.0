#include <iostream>
#include <vector>
#include "TZE.h"


class ClientApp : public tze::App
{
public:
	ClientApp() : App()
	{
		// std::vector<glm::vec3> _colors
		// {
		// 	{255, 0, 0},
		// 	{204, 102, 0},
		// 	{255, 255, 0},
		// 	{0, 204, 0},
		// 	{0, 0, 204},
		// 	{102, 0, 104},
		// 	{153, 0, 153},
		// };
		// 
		// for (int i = _colors.size(); i > 0; i--)
		// {
		// 	std::vector<tze::Model::Vertex> vertices // deciding what will be the location of the triangle on the screen:
		// 	{
		// 		{{i * 0.1f, i * -0.1f }},
		// 		{{i * 0.1f, i * 0.1f  }},
		// 		{{i * -0.1f,i * -0.1f}}
		// 	};
		// 	makeTriangle(vertices, _colors[i - 1]);
		// }

		// std::vector<tze::Model::BasicVertex> vertices{
		// {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		// {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		// {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}} };
		// 
		// auto model = std::make_shared<tze::Model>(getPhysicalDevice(), getLogicalDevice(), vertices);
		// 
		// // https://www.color-hex.com/color-palette/5361
		// std::vector<glm::vec3> colors{
		// 	{1.f, .7f, .73f},
		// 	{1.f, .87f, .73f},
		// 	{1.f, 1.f, .73f},
		// 	{.73f, 1.f, .8f},
		// 	{.73, .88f, 1.f}  
		// };
		// for (auto& color : colors) {
		// 	color = glm::pow(color, glm::vec3{ 2.2f });
		// }
		// for (int i = 0; i < 0; i++) {
		// 	auto triangle = tze::GameObject::p_createGameObj();
		// 	triangle->_model = model;
		// 	triangle->_transform_2D._scale = glm::vec2(.5f) + i * 0.025f;
		// 	triangle->_transform_2D._rotation = i * glm::pi<float>() * .025f;
		// 	triangle->_color = colors[i % colors.size()];
		// 	_renderer.addGameObjects(std::move(triangle));
		// }

		std::vector<tze::Model::BasicVertex> vertices{
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}};
		auto model = std::make_shared<tze::Model>(getPhysicalDevice(), getLogicalDevice(), vertices);
		auto triangle = tze::GameObject::p_createGameObj();
		triangle->_model = model;
		triangle->_transform_2D._scale = glm::vec2(.5f) * 0.025f;
		triangle->_transform_2D._rotation = glm::pi<float>() * .025f;
		triangle->_color = { 1.f, .7f, .73f };
		_renderer.addGameObjects(std::move(triangle));
	}
	~ClientApp()
	{
	}

	void makeTriangle(const std::vector<tze::Model::BasicVertex>& vertex, const glm::vec3& color)
	{
		//  constructing the game object:
		tze::GameObject* triangle = tze::GameObject::p_createGameObj();
		std::vector<tze::Model::BasicVertex> vertices // deciding what will be the location of the triangle on the screen:
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