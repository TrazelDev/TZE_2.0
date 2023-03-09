#pragma once
#include "pch.h"
namespace tze
{
	class TZE_API Model
	{
	public:
		struct Vertex
		{
			glm::vec2 _position;
			glm::vec3 _color;

			// define later
			static std::vector<vk::VertexInputBindingDescription> getBindingDescriptions();
			// define later
			static std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions();
		};

		Model(vk::PhysicalDevice& physicalDevice, vk::Device& device, std::vector<Vertex>& vertices);
		~Model();
		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void bind(const vk::CommandBuffer& commandBuffer);
		void draw(const vk::CommandBuffer& commandBuffer);

	private:
		void createVertexBuffer(const std::vector<Vertex>& vertices);
		/// <summary>
		/// this function is creating a buffer of the data on the gpu that is used to speed things up
		/// </summary>
		/// <param name="size">the size of the buffer that is going to be created</param>
		/// <param name="flags">what kind of buffer it is going to be most common is vk::BufferUsageFlagBits::eVertexBuffer for vertex buffer type</param>
		/// <param name="properties"></param>
		void createBuffer
		(
			const vk::DeviceSize& size,
			const vk::BufferUsageFlags& flags,
			const vk::MemoryPropertyFlags& properties
		);

		/// <summary>
		/// this function is checking for an index that suits the properties filter type that are going to be provided as input 
		/// </summary>
		/// <param name="typeFilter">the bit mask of bits that are going to be filtered </param>
		/// <param name="properties">the properties that are need for the buffer in the gpu</param>
		/// <returns></returns>
		uint32_t findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& properties);

		vk::Device& _logicalDevice;
		vk::PhysicalDevice& _physicalDevice;
		vk::Buffer _vertexBuffer;
		vk::DeviceMemory _vertexBufferMemory;
		uint32_t _vertexCount;
	};
}
