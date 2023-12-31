#pragma once

#include <GL/glew.h>
#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
			case GL_UNSIGNED_SHORT:
				return 2;
		}
		__debugbreak();
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;

public:
	VertexBufferLayout()
		: m_stride(0u) {}

	template<typename T>
	void Push(unsigned int count)
	{
		__debugbreak();
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<uint32_t>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<uint8_t>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	template<>
	void Push<uint16_t>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_SHORT, count, GL_FALSE });
		m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_SHORT) * count;
	}

	const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
	unsigned int GetStride() const { return m_stride; }
};

