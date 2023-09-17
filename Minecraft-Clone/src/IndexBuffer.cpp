#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :m_count(count)
{
    GLCall(glGenBuffers(1, &m_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::Release()
{
    GLCall(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::SetData(const unsigned int* data, unsigned int count)
{
    m_count = count;
    GLCall(glGenBuffers(1, &m_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
