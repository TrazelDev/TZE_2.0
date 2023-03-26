#include "pch.h"
#include "GameObj.h"

tze::Transform_2DComponent::Transform_2DComponent(const glm::vec2& scale, float rotation)
{
	_scale = scale;
	_rotation = rotation;
}

tze::Transform_2DComponent::Transform_2DComponent(float rotation, const glm::vec2& scale)
{
	_scale = scale;
	_rotation = rotation;
}

glm::mat2 tze::Transform_2DComponent::mat2()
{
	const float s = glm::sin(_rotation);
	const float c = glm::cos(_rotation);

	glm::mat2 rotationMatrix{ {c,s}, {-s, c} }; // this is a way to make a matrix that is rotating the object by the amount of radiens that were taken as input
	glm::mat2 scaleMatrix{ {_scale.x, 0.0f}, {0.0f, _scale.y} };

	return (rotationMatrix * scaleMatrix);
}

void tze::Transform_2DComponent::runChanges()
{
	if (_spin)
	{
		_rotation = glm::mod(_rotation + +0.0005f, glm::two_pi<float>());
	}
}

tze::GameObject tze::GameObject::createGameObj()
{
	static id_t currId = 0;
	return GameObject(currId++);
}

tze::GameObject* tze::GameObject::p_createGameObj()
{
	static id_t currId = 0;
	return new GameObject(currId++);
}

void tze::GameObject::run()
{
}