#include "pch.h"
#include "Model.h"

tze::Model::Model(vk::PhysicalDevice& physicalDevice, vk::Device& logicalDevice, std::vector<Vertex>& vertices) 
	: _physicalDevice(physicalDevice), _logicalDevice(logicalDevice)
{
	createVertexBuffer(vertices);
}

tze::Model::~Model()
{
	_logicalDevice.waitIdle();
	_logicalDevice.destroyBuffer(_vertexBuffer, nullptr);
	_logicalDevice.freeMemory(_vertexBufferMemory, nullptr);
}

void tze::Model::bind(const vk::CommandBuffer& commandBuffer)
{
	vk::DeviceSize offsets[] = { 0 };
	commandBuffer.bindVertexBuffers(0, 1, &_vertexBuffer, offsets);
}

void tze::Model::draw(const vk::CommandBuffer& commandBuffer)
{
	commandBuffer.draw(_vertexCount, 1, 0, 0);
}

void tze::Model::createVertexBuffer(const std::vector<Vertex>& vertices)
{
	_vertexCount = uint32_t(vertices.size());
	assert(_vertexCount >= 3 && "vertex count must be at least 3");
	vk::DeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;
	
	createBuffer
	(
		bufferSize,
		vk::BufferUsageFlagBits::eVertexBuffer,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
	);

	// we map the memory to the cpu to get the address then we copy it and then unMap it save place on the cpu:
	void* data = _logicalDevice.mapMemory(_vertexBufferMemory, 0, bufferSize, vk::MemoryMapFlags());
	memcpy(data, vertices.data(), bufferSize);
	_logicalDevice.unmapMemory(_vertexBufferMemory);
}


void tze::Model::createBuffer(const vk::DeviceSize& size, const vk::BufferUsageFlags& flags, const vk::MemoryPropertyFlags& properties)
{
	// the settings to a buffer that is going to be created on the gpu:
	vk::BufferCreateInfo bufferInfo = {};
	bufferInfo.sType = vk::StructureType::eBufferCreateInfo; // the most common choice not really important
	bufferInfo.size = size; // the size of the buffer that is going to be created and used
	bufferInfo.usage = flags;
	// the types of sharing modes ( vk::SharingMode::eConcurrent - if the buffer can be used by a multiple family queues for example rendering and graphics
	// and vk::SharingMode::eExclusive if the buffer can only be used by one type of family queue)
	bufferInfo.sharingMode = vk::SharingMode::eExclusive; 
	bufferInfo.pNext = nullptr; // this filed is not commonly used but provides a pointer to an extension structure of this structure
	// another options:
	// bufferInfo.queueFamilyIndexCount = 0; // if the option vk::SharingMode::eConcurrent than it is a field that provides how many queue families can use the buffer
	// bufferInfo.pQueueFamilyIndices = nullptr; // a pointer to an array of queue family indices that will be using the buffer 

	// creating the actual buffer to later be put in the gpu memory
	if (_logicalDevice.createBuffer(&bufferInfo, nullptr, &_vertexBuffer) != vk::Result::eSuccess) 
	{
		TZE_ENGINE_ERR("Failed to create a buffer");
	}


	vk::MemoryRequirements memRequirements;
	_logicalDevice.getBufferMemoryRequirements(_vertexBuffer, &memRequirements);
	vk::MemoryAllocateInfo allocationInfo = {};
	allocationInfo.sType = vk::StructureType::eMemoryAllocateInfo;
	allocationInfo.allocationSize = memRequirements.size;
	allocationInfo.memoryTypeIndex= findMemoryType(memRequirements.memoryTypeBits, properties);
	
	
	if (_logicalDevice.allocateMemory(&allocationInfo, nullptr, &_vertexBufferMemory) != vk::Result::eSuccess)
	{
		TZE_ENGINE_ERR("failed to allocate memory in the gpu");
	}
	
	_logicalDevice.bindBufferMemory(_vertexBuffer, _vertexBufferMemory, 0);
}

uint32_t tze::Model::findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& properties) {
	vk::PhysicalDeviceMemoryProperties memProperties;
	_physicalDevice.getMemoryProperties(&memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
	{
		// condition 1: since the typeFilter is a bitmask we make a right bit shift and checking if the bit is turned on
		// condition 2: making an AND bitwise operation with the wanted properties and the specified properties and checking 
		// if any of the bits were turned of if not then the condition is true
		// and returning the index of this place in the memory of the gpu
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
		{
			return i;
		}
	}

	TZE_ENGINE_ERR("failed to find a place for this buffer properties");
	return -1;
}

std::vector<vk::VertexInputBindingDescription> tze::Model::Vertex::getBindingDescriptions()
{
	std::vector<vk::VertexInputBindingDescription> bidingDescription(1);
	bidingDescription[0].binding = 0;
	bidingDescription[0].stride = sizeof(Vertex);
	bidingDescription[0].inputRate = vk::VertexInputRate::eVertex;


	return bidingDescription;
}

std::vector<vk::VertexInputAttributeDescription> tze::Model::Vertex::getAttributeDescriptions()
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescription(2);
	attributeDescription[0].binding = 0;
	attributeDescription[0].location = 0;
	attributeDescription[0].format = vk::Format::eR32G32Sfloat; // VK_FORMAT_R32G32_SFLOAT;
	attributeDescription[0].offset = offsetof(Vertex, _position);

	attributeDescription[1].binding = 0;
	attributeDescription[1].location = 1;
	attributeDescription[1].format = vk::Format::eR32G32Sfloat;
	attributeDescription[1].offset = offsetof(Vertex, _color);

	return attributeDescription;
}
