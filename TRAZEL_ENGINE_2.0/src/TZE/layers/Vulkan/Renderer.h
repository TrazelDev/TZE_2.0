#pragma once
#include "Swapchain.h"
#include "Objects/GameObj.h"
#include "pipeline/Pipeline.h"
#include "layers/Window/Window.h"
#include "Commands.h"

namespace tze
{
	struct RendererInput
	{
		vk::Device& _logicalDevice;
		QueueFamilies& _indices;
		Window& _window;
		Swapchain& _swapchain;

		RendererInput(
			vk::Device& logicalDevice,
			QueueFamilies& indices,
			Window& window,
			Swapchain& swapchain
		)
			:
			_logicalDevice(logicalDevice),
			_swapchain(swapchain),
			_window(window),
			_indices(indices)
		{ }
	};

	class TZE_API Renderer
	{
	public:
		Renderer(const RendererInput& input);
		~Renderer();

		/// <summary>
		/// this function is being called when the size of the window has changed and the current swapchain and pipeline needs to 
		/// be reconstructed because they don't suit the new size
		/// </summary>
		void addGameObjects(GameObject* obj);
		inline void run();
	private:
		inline void recordDrawCommands(const vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
		inline void renderGameObj(const vk::CommandBuffer& commandBuffer);
		void recreationOfWindowSize();

		const RendererInput& _input;
		Pipeline  _pipeline;
		Commands  _commands;
		std::vector<GameObject*> _gameObjects;
		vk::Queue _graphicsQueue;
		vk::Queue _presentQueue;
		uint32_t _maxFramesInFlight;
		uint32_t _frameNum;
	};
}
