#include "pinepch.h"
#include "Buffers.h"

#include <glad\glad.h>

namespace Pine {

	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
		: m_Elements(elements)
	{
		PINE_PROFILE_FUNCTION();

		CalculateOffsetsAndStride();
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		PINE_PROFILE_FUNCTION();

		size_t offset = 0;
		uint32_t stride = 0;

		for (auto& element : m_Elements) {
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	VertexBuffer::VertexBuffer(float* vertices, size_t size, const BufferLayout& layout)
		: m_Layout(layout)
	{
		PINE_PROFILE_FUNCTION();

		glGenBuffers(1, &m_BufferId);

		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(size_t size, const BufferLayout& layout)
		: m_Layout(layout)
	{
		PINE_PROFILE_FUNCTION();

		glGenBuffers(1, &m_BufferId);

		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		PINE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_BufferId);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		PINE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		PINE_PROFILE_FUNCTION();

		glGenBuffers(1, &m_BufferId);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		PINE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_BufferId);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
