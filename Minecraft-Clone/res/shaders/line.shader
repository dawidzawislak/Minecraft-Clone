#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 uProjMat;
uniform mat4 uViewMat;

void main()
{
	gl_Position = uProjMat* uViewMat* vec4(position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec3 uColor;

void main()
{
	FragColor = vec4(uColor, 1.0);
};