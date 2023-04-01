#pragma once
#include "layers/Layer.h"
#include "layers/Vulkan/vk_logging/vkLogging.h"




namespace tze
{
	struct pipelineInput
	{
		vk::Device& _logicalDevice;
		vk::Format& _swapchainImageFormat;
		vk::Extent2D& _swapchainExtent;

		pipelineInput(vk::Device& logicalDevice, vk::Format& swapchainImageFormat, vk::Extent2D& swapchainExtent) :
			_logicalDevice(logicalDevice), _swapchainImageFormat(swapchainImageFormat), _swapchainExtent(swapchainExtent) {}
	};

	/// <summary>
	/// this is a struct that describes the data that is going to be transferred to the fragment shader
	/// </summary>
	struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.0f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	class Pipeline : public Layer
	{
	public:
		Pipeline(const pipelineInput& input,
			const std::string& vertexShaderPath = /*"src\\TZE\\layers\\Vulkan\\pipeline\\Shaders\\*/"vertex.spv"
			, const std::string& fragmentShaderPath = /*"src\\TZE\\layers\\Vulkan\\pipeline\\Shaders\\*/"fragment.spv");
		~Pipeline();

		void run() override;
		/// <summary>
		/// function that is in charge of creating a new pipeline variables based on the new extent
		/// </summary>
		void recreatePipeline();

		vk::RenderPass& getRenderPass();
		vk::Pipeline& getPipeline();
		vk::PipelineLayout& getLayout();
	private:
		/// <summary>
		/// function is getting a file name and base on the information in the file creating a vulkan shader modul
		/// </summary>
		/// <param name="filename">the name of the file shader with the information</param>
		/// <returns>the vulkan shader module corresponding to data inside the shader spv file</returns>
		vk::ShaderModule createModule(const std::string& filename);

		/// <summary>
		/// function that is getting a file name and putting the char of the file into a vector of chars
		/// </summary>
		/// <param name="filename">the name of the file we want to check</param>
		/// <returns>the vector of chars that contains the info of the file</returns>
		std::vector<char> readFile(const std::string& filename);

		// this function is getting the logical device and creating a pipeline layout from it and returning it
		// pipeline layout is a way to connect the pipeline shader stage to the other stages
		vk::PipelineLayout makePipelineLayout();

		/// <summary>
		/// the function is creating a renderPass ( renderPass - )
		/// </summary>
		/// <returns>renderPass is has created</returns>
		vk::RenderPass makeRenderPass();

		const pipelineInput& _input;
		std::string _vertexShaderPath;
		std::string _fragmentShaderPath;

		// pipeline related:
		vk::PipelineLayout _layout;
		vk::RenderPass _renderPass;
		vk::Pipeline _graphicsPipeline;
	};
}
