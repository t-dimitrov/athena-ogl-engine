#pragma once
#include "Core/Ref.h"
#include "Core/Core.h"
#include <vector>

#include <glad/glad.h>

namespace Athena
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RGBA16,
		RGBA32,

		// Depth/stencil
		DEPTH32F_STENCIL8,

		// Defaults
		Color = RGBA8,
		Depth = DEPTH32F_STENCIL8
	};
	
	struct FramebufferAttachmentDescriptor
	{
		FramebufferAttachmentDescriptor() = default;
		FramebufferAttachmentDescriptor(FramebufferTextureFormat format) : format(format) {}

		FramebufferTextureFormat format;
	};

	struct FramebufferDescriptor
	{
		uint32_t width = 1600;
		uint32_t height = 900;
		uint32_t samples = 1; // multisampling

		std::vector<FramebufferAttachmentDescriptor> attachments;

		std::string debugName;
	};

    class Framebuffer : public RefCounted
    {
	public:
		Framebuffer(const FramebufferDescriptor& descriptor);
		~Framebuffer();

		void Bind() const;
		void Unbind() const;

		void Resize(uint32_t width, uint32_t height);

		inline FramebufferDescriptor GetDescriptor() const { return _descriptor; }
		inline uint32_t GetColorAttachmentId(uint32_t index = 0) const 
		{ 
			ATH_ASSERT(index < _colorAttachmentIds.size(), "Index out of bounds"); 
			return _colorAttachmentIds[index]; 
		}
		inline uint32_t GetDepthAttachmentId() const
		{
			return _depthAttachmentId;
		}

	private:
		void Recreate();
		void AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index);
		void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height);

	private:
		uint32_t _framebufferId = 0;
		std::vector<uint32_t> _colorAttachmentIds;
		uint32_t _depthAttachmentId;

		std::vector<FramebufferAttachmentDescriptor> _colorAttachmentDescriptors;
		FramebufferAttachmentDescriptor _depthAttachmentDescriptor;

		FramebufferDescriptor _descriptor;
    };
}
