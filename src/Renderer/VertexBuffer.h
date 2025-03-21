#pragma once
#include "Core/Ref.h"
#include "Core/Core.h"

#include <glad/glad.h>
#include <vector>

namespace Athena
{
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;

			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;

			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;

			case ShaderDataType::Bool:     return 1;
		}

		ATH_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct VertexBufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t byteSize;
		uint32_t offset;
		bool normalized;

		VertexBufferElement() = default;

		VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: type(type)
			, name(name)
			, byteSize(ShaderDataTypeSize(type))
			, offset(0)
			, normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;

				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;

				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;

				case ShaderDataType::Bool:    return 1;
			}

			ATH_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

		GLenum GetShaderDataType() const
		{
			switch (type)
			{
				case ShaderDataType::Float:   return GL_FLOAT;
				case ShaderDataType::Float2:  return GL_FLOAT;
				case ShaderDataType::Float3:  return GL_FLOAT;
				case ShaderDataType::Float4:  return GL_FLOAT;

				case ShaderDataType::Int:     return GL_INT;
				case ShaderDataType::Int2:    return GL_INT;
				case ShaderDataType::Int3:    return GL_INT;
				case ShaderDataType::Int4:    return GL_INT;

				case ShaderDataType::Mat3:    return GL_FLOAT;
				case ShaderDataType::Mat4:    return GL_FLOAT;

				case ShaderDataType::Bool:    return GL_BYTE;
			}

			ATH_ASSERT(false, "Unknown ShaderDataTyp!");
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
		uint32_t GetStride() const { return m_Stride; }

		std::vector<VertexBufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<VertexBufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<VertexBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<VertexBufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.offset = m_Stride;
				m_Stride += element.byteSize;
			}
		}

	private:
		std::vector<VertexBufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

    class VertexBuffer : public RefCounted
    {
    public:
		VertexBuffer(void* data, uint32_t byteSize);
		VertexBuffer(const void* data, uint32_t byteSize);
        ~VertexBuffer();

        void Bind() const;

		void SetLayout(const VertexBufferLayout& layout) const;

    private:
        uint32_t _bufferId;
    };
}
