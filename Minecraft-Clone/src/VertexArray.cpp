#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
	m_id = 0;
}

VertexArray::~VertexArray()
{
	if (m_id != 0)
		Release();
}
void VertexArray::Release()
{
	if (m_id != 0) {
		GLCall(glDeleteVertexArrays(1, &m_id));
		m_id = 0;
	}
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	if(!m_id)
		GLCall(glGenVertexArrays(1, &m_id));
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		GLCall(glEnableVertexAttribArray(i));
		if (elements[i].type == GL_UNSIGNED_BYTE || elements[i].type == GL_UNSIGNED_SHORT || elements[i].type == GL_UNSIGNED_INT) {
			GLCall(glVertexAttribIPointer(i, elements[i].count, elements[i].type, layout.GetStride(), (const void*)offset));
		}
		else {
			GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (const void*)offset));
		}
		offset += elements[i].count * VertexBufferElement::GetSizeOfType(elements[i].type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_id));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
