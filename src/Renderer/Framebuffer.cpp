#include "Framebuffer.h"

namespace Athena
{
    static const uint32_t s_maxFramebufferSize = 8192;

    Framebuffer::Framebuffer(const FramebufferDescriptor& descriptor)
        : _descriptor(descriptor)
    {
        for (const auto& attachment : _descriptor.attachments)
        {
            if (attachment.format == FramebufferTextureFormat::DEPTH24)
            {
                _depthAttachmentDescriptor = attachment;
            }
            else
            {
                _colorAttachmentDescriptors.emplace_back(attachment);
            }
        }

        Recreate();
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &_framebufferId);
        glDeleteTextures(static_cast<GLsizei>(_colorAttachmentIds.size()), _colorAttachmentIds.data());
        glDeleteTextures(1, &_depthAttachmentId);
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
        glViewport(0, 0, _descriptor.width, _descriptor.height);
    }

    void Framebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > s_maxFramebufferSize || height > s_maxFramebufferSize)
        {
            LOG_WARN("Framebuffer can't be resized to ({}, {})", width, height);
            return;
        }

        _descriptor.width = width;
        _descriptor.height = height;

        Recreate();
    }

    void Framebuffer::Recreate()
    {
		if (_framebufferId)
		{
			glDeleteFramebuffers(1, &_framebufferId);
			glDeleteTextures(static_cast<GLsizei>(_colorAttachmentIds.size()), _colorAttachmentIds.data());
			glDeleteTextures(1, &_depthAttachmentId);

			_colorAttachmentIds.clear();
			_depthAttachmentId = 0;
		}

		glGenFramebuffers(1, &_framebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);

		bool multisample = _descriptor.samples > 1;
		GLenum textureTarget = multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

		// Create color attachments
		if (_colorAttachmentDescriptors.size())
		{
			_colorAttachmentIds.resize(_colorAttachmentDescriptors.size());
			glCreateTextures(textureTarget, static_cast<GLsizei>(_colorAttachmentIds.size()), _colorAttachmentIds.data());

			for (size_t i = 0; i < _colorAttachmentDescriptors.size(); i++)
			{
				glBindTexture(textureTarget, _colorAttachmentIds[i]);

				switch (_colorAttachmentDescriptors[i].format)
				{
				case FramebufferTextureFormat::RGBA8:
					AttachColorTexture(_colorAttachmentIds[i], _descriptor.samples, GL_RGBA8, _descriptor.width, _descriptor.height, (int)i);
					break;
				}
			}
		}

		// Create depth attachment
		if (_depthAttachmentDescriptor.format != FramebufferTextureFormat::None)
		{
			glCreateTextures(textureTarget, 1, &_depthAttachmentId);
			glBindTexture(textureTarget, _depthAttachmentId);

			switch (_depthAttachmentDescriptor.format)
			{
			case FramebufferTextureFormat::DEPTH24:
				AttachDepthTexture(_depthAttachmentId, _descriptor.samples, GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT, _descriptor.width, _descriptor.height);
				break;
			}
		}

		if (_colorAttachmentIds.size() > 1)
		{
			ATH_ASSERT(_colorAttachmentIds.size() <= 4, "Only four color attachments are allowed!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(static_cast<GLsizei>(_colorAttachmentIds.size()), buffers);
		}
		else if (_colorAttachmentIds.empty())
		{
			// Only depth pass
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		ATH_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

	void Framebuffer::AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		GLenum textureTarget = multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		if (multisampled)
		{
			glTexImage2DMultisample(textureTarget, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(textureTarget, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget, id, 0);
	}

	void Framebuffer::AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		GLenum textureTarget = multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		if (multisampled)
		{
			glTexImage2DMultisample(textureTarget, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(textureTarget, 1, format, width, height);

			glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget, id, 0);
	}
}
