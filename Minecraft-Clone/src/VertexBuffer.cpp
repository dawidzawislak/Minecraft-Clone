#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_id));
}

void VertexBuffer::Release()
{
    GLCall(glDeleteBuffers(1, &m_id));
}

void VertexBuffer::SetData(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
