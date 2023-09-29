#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;

out vec2 fTexCoords;

uniform mat4 uProjMatrix;

void main()
{
	fTexCoords = aTexCoords;
	gl_Position = uProjMatrix * vec4(aPosition, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 fTexCoords;

uniform sampler2D uTexture;

void main()
{
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	FragColor = texture(uTexture, fTexCoords);
};