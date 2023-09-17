#pragma once

class IndexBuffer
{
private:
	unsigned int m_id;
	unsigned int m_count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer() {};
	~IndexBuffer();

	void SetData(const unsigned int* data, unsigned int count);
	void Release();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_count; }
};

