#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	std::string m_filePath;
	unsigned int m_id;
	std::unordered_map<std::string, int> m_uniformLocationCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void SetUniform1i(const std::string& name, int value);

private:
	unsigned int CompileShader(const std::string& source, unsigned int type);
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CreateShader(ShaderProgramSource source);
	unsigned int GetUniformLocation(const std::string& name);
};

