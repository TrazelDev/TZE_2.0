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
		void recreatePipeline(vk::Extent2D& swapchainExtent);

		vk::RenderPass& getRenderPass();
		vk::Pipeline& getPipeline();
		vk::PipelineLayout& getLayout();
	private:
		/// <summary>
		/// this function is in charge of creating the pipeline it is basically creating all of the class almost
		/// </summary>
		/// <param name="swapchainExtent"></param>
		void createPipeLine(vk::Extent2D& swapchainExtent);
		void destroyPipeline(); // destroying almost evreything in the class

		inline vk::PipelineVertexInputStateCreateInfo createVertexInputInfo() const;
		/// <summary>
		/// sets the basic topology that the vertices for now planning to support the following:
		/// 
		/// <para> ePointList: This topology is used for drawing individual points. 
		/// It's commonly used for particle systems or drawing small objects such as UI elements. </para>
		/// 
		/// <para> eLineList: This topology is used for drawing individual lines.
		/// It's useful for drawing shapes or connecting points.</para>
		/// 
		/// <para> eLineStrip : This topology is similar to eLineList, but it creates a single connected line.
		/// It's useful for drawing continuous curves or strokes.</para>
		/// 
		/// <para> eTriangleList : This topology is used for drawing individual triangles.
		/// It's useful for drawing filled shapes such as polygons. </para>
		/// 
		/// <para> eTriangleStrip : This topology is similar to eTriangleList, but it creates a single connected triangle strip.
		/// It's useful for drawing complex surfaces or terrains.</para>
		/// 
		/// </summary>
		/// <param name="topology">vk::PrimitiveTopology::ePointList or vk::PrimitiveTopology::eLineList or 
		/// vk::PrimitiveTopology::eTriangleList or vk::PrimitiveTopology::eTriangleList or vk::PrimitiveTopology::eTriangleStrip </param>
		/// <returns></returns>
		inline vk::PipelineInputAssemblyStateCreateInfo createInputAssemblyInfo(const vk::PrimitiveTopology topology) const;
		// inline std::vector<vk::PipelineShaderStageCreateInfo> createShaderStages() const;
		inline std::vector<vk::PipelineShaderStageCreateInfo> createShaderStages() const;

		/// <summary>
		/// function is getting a file name and base on the information in the file creating a vulkan shader modul
		/// </summary>
		/// <param name="filename">the name of the file shader with the information</param>
		/// <returns>the vulkan shader module corresponding to data inside the shader spv file</returns>
		vk::ShaderModule createModule(const std::string& filename) const;

		/// <summary>
		/// function that is getting a file name and putting the char of the file into a vector of chars
		/// </summary>
		/// <param name="filename">the name of the file we want to check</param>
		/// <returns>the vector of chars that contains the info of the file</returns>
		std::vector<char> readFile(const std::string& filename) const;

		// this function is getting the logical device and creating a pipeline layout from it and returning it
		// pipeline layout is a way to connect the pipeline shader stage to the other stages
		vk::PipelineLayout makePipelineLayout();

		/// <summary>
		/// the function is creating a renderPass 
		/// </summary>
		/// <returns>renderPass that has created</returns>
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
