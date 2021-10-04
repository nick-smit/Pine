#include "pinepch.h"
#include "VertexArray.h"

#include <glad\glad.h>

namespace Pine {
	namespace Utils {
		static GLenum PineTypeToGlType(ShaderDataType type)
		{
			switch (type) {
				case ShaderDataType::Float1:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
					return GL_FLOAT;
				case ShaderDataType::Int1:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
					return GL_INT;
				case ShaderDataType::Bool1:
					return GL_BOOL;
			}

			PINE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	}

	VertexArray::VertexArray()
	{
		PINE_PROFILE_FUNCTION();

		glGenVertexArrays(1, &m_VertexArrayId);
	}

	VertexArray::~VertexArray()
	{
		PINE_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_VertexArrayId);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_VertexArrayId);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		PINE_PROFILE_FUNCTION();

		glBindVertexArray(m_VertexArrayId);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			switch (element.Type) {
				case ShaderDataType::Float1:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(
						m_VertexBufferIndex,
						element.GetComponentCount(),
						Utils::PineTypeToGlType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset
					);
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Int1:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool1:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribIPointer(
						m_VertexBufferIndex,
						element.GetComponentCount(),
						Utils::PineTypeToGlType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset
					);
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++) {
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribPointer(
							m_VertexBufferIndex,
							count,
							Utils::PineTypeToGlType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i)
						);
						glVertexAttribDivisor(m_VertexBufferIndex, 1);
						m_VertexBufferIndex++;
					}
					break;
				}
				default: PINE_ASSERT(false, "Unknown ShaderDataType");
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> IndexBuffer)
	{
		PINE_PROFILE_FUNCTION();

		glBindVertexArray(m_VertexArrayId);
		IndexBuffer->Bind();

		m_IndexBuffer = IndexBuffer;
	}

}
