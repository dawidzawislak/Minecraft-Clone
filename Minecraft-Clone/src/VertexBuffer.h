#pragma once

class VertexBuffer
{
private:
	unsigned int m_id;

public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer() {}
	~VertexBuffer();

	void SetData(const void* data, unsigned int size);

	void Bind() const;
	void Unbind() const;
};

