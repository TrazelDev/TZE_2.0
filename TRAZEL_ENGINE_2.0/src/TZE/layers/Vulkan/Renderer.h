#pragma once
#include "Swapchain.h"
#include "Objects/GameObj.h"
#include "pipeline/Pipeline.h"

namespace tze
{
	struct RendererInput
	{
		vk::Device& _logicalDevice;
		vk::SwapchainKHR             _swapchain;
		std::vector<SwapchainFrame>& _swapchainFrames;
		vk::RenderPass& _renderpass;
		vk::Extent2D& _extent;
		uint32_t					 _width;
		uint32_t					 _height;
		vk::Pipeline& _pipeline;
		vk::PipelineLayout& _layout;
		QueueFamilies& _indices;

		RendererInput(vk::Device& logicalDevice,
			vk::SwapchainKHR& swapchain,
			std::vector<SwapchainFrame>& swapchainFrames,
			vk::RenderPass& renderpass,
			vk::Extent2D& extent,
			uint32_t					 width,
			uint32_t					 height,
			vk::Pipeline& pipeline,
			vk::PipelineLayout& layout,
			QueueFamilies& indices) :
			_logicalDevice(logicalDevice),
			_swapchain(swapchain),
			_swapchainFrames(swapchainFrames),
			_renderpass(renderpass),
			_extent(extent),
			_width(width),
			_height(height),
			_pipeline(pipeline),
			_layout(layout),
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
