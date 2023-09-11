#pragma once

#include <string>

class Texture
{
private:
	unsigned int m_id;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_bpp;

public:
	Texture(std::string filePath);
	Texture(unsigned char* data, int width, int height, int bpp);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
};