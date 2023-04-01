#pragma once
#include "Swapchain.h"
#include "Objects/GameObj.h"
#include "pipeline/Pipeline.h"

namespace tze
{
	struct RendererInput
	{
		vk::Device& _logicalDevice;
		uint32_t					 _width;
		uint32_t					 _height;
		QueueFamilies& _indices;
		Pipeline& _pipeline;
		Swapchain& _swapchain;

		RendererInput(
			uint32_t					 width,
			uint32_t					 height,
			vk::Device& logicalDevice,
			QueueFamilies& indices,
			Swapchain& swapchain,
			Pipeline& pipeline
		) 
			:
			_logicalDevice(logicalDevice),
			_swapchain(swapchain),
			_pipeline(pipeline),
			_width(width),
			_height(height),
			_indices(indices)
		{ }
	};

	class TZE_API Renderer
	{
	public:
		Renderer(const RendererInput& input);
		~Renderer();

		void addGameObjects(GameObject* obj);
		void run();
	private:
		void recordDrawCommands(const vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
		void renderGameObj(const vk::CommandBuffer& commandBuffer);

		const RendererInput& _input;
		std::vector<GameObject*> _gameObjects;
		vk::Queue _graphicsQueue;
		vk::Queue _presentQueue;
		uint32_t _maxFramesInFlight;
		uint32_t _frameNum;
	};
}
