#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filePath)
	:m_filePath(filePath), m_id(0u)
{
    ShaderProgramSource source = ParseShader(m_filePath);
    m_id = CreateShader(source);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_id))
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_id));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform3f(const std::string& name, float v1, float v2, float v3)
{
    GLCall(glUniform3f(GetUniformLocation(name), v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
{
    GLCall(glUniform4f(GetUniformLocation(name), v1, v2, v3, v4));
}

void Shader::SetUniform2i(const std::string& name, int v1, int v2)
{
    GLCall(glUniform2i(GetUniformLocation(name), v1, v2));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = new char[length];

        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        delete[] message;
        return 0;
    }
	return id;
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_id, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    
    m_uniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    if (stream.fail()) {
        std::cout << "Failed to open: " << filePath << std::endl;
        return { "", "" };
    }

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];

    ShaderType mode = ShaderType::NONE;

    while (std::getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                mode = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                mode = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)mode] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(ShaderProgramSource source)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(source.vertexSource, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(source.fragmentSource, GL_FRAGMENT_SHADER);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));

    int result;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char* message = new char[length];

        GLCall(glGetProgramInfoLog(program, length, &length, message));
        std::cout << "Failed to link shaders!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteProgram(program));
        delete[] message;
        return 0;

    }
    GLCall(glDetachShader(program, vs));
    GLCall(glDetachShader(program, fs));
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}