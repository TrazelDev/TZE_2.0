#pragma once
#include "layers/Layer.h"
#include "Model.h"

namespace tze
{
	struct TZE_API Transform_2DComponent
	{
		glm::vec2 _scale{ 1.0f, 1.0f }; // default - no scaling 
		float _rotation; // the degree of rotation in radians ( the rotation is counterclockwise )

		Transform_2DComponent(const glm::vec2& scale = { 1.0f, 1.0f }, float rotation = 0);
		Transform_2DComponent(float rotation, const glm::vec2& scale = { 1.0f, 1.0f }); // exists to make a way to only give a rotation

		/// <summary>
		/// calculating the matrix that needs to be used based on the the parameters of the object and returns the matrix
		/// </summary>
		glm::mat2 mat2();
	};

	class TZE_API GameObject : public Layer
	{
		using id_t = unsigned int;
	public:
		static GameObject createGameObj();
		static GameObject* p_createGameObj();

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		id_t getId() { return id; }
		
		void run() override;

		std::shared_ptr<Model> model;
		glm::vec3 color;
		Transform_2DComponent transform_2D;

	private:
		GameObject(id_t objId) : id(objId), transform_2D() {}
		id_t id;
	};
}