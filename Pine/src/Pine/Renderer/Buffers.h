#pragma once

#include "Pine\Core\Core.h"

#include <string>
#include <vector>

namespace Pine {

	enum class ShaderDataType
	{
		None = 0, Float1, Float2, Float3, Float4, Mat3, Mat4, Int1, Int2, Int3, Int4, Bool1
	};

	namespace Utils {
		static uint32_t ShaderDataTypeSize(ShaderDataType type)
		{
			switch (type)
			{
				case ShaderDataType::Float1:   return 4;
				case ShaderDataType::Float2:   return 4 * 2;
				case ShaderDataType::Float3:   return 4 * 3;
				case ShaderDataType::Float4:   return 4 * 4;
				case ShaderDataType::Mat3:     return 4 * 3 * 3;
				case ShaderDataType::Mat4:     return 4 * 4 * 4;
				case ShaderDataType::Int1:     return 4;
				case ShaderDataType::Int2:     return 4 * 2;
				case ShaderDataType::Int3:     return 4 * 3;
				case ShaderDataType::Int4:     return 4 * 4;
				case ShaderDataType::Bool1:    return 1;
			}

			PINE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	}

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(Utils::ShaderDataTypeSize(Type)), Offset(0), Normalized(normalized) {};

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float1:  return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3;
				case ShaderDataType::Mat4:    return 4;
				case ShaderDataType::Int1:    return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool1:   return 1;
			}

			PINE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

	};

	class BufferLayout {
	public:
		BufferLayout(std::initializer_list<BufferElement> elements);
		virtual ~BufferLayout() = default;

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride();

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer {
	public:
		VertexBuffer(float* vertices, size_t size, const BufferLayout& layout);
		VertexBuffer(size_t size, const BufferLayout& layout);
		virtual ~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t size);

		const BufferLayout& GetLayout() const { return m_Layout; }

	private:
		uint32_t m_BufferId;
		BufferLayout m_Layout;
	};

	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_BufferId;
		uint32_t m_Count;
	};

}