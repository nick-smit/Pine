#pragma once

#include "Buffers.h"

#include <memory>
#include <vector>

namespace Pine {

	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const VertexArray& other) = delete;
		virtual ~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_VertexArrayId;

		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		uint32_t m_VertexBufferIndex = 0;

		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
