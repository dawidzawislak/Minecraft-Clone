#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in uint aFace;


out vec2 fTexCoord;
flat out uint fFace;
out vec3 fFragPosition;

uniform mat4 uProjMat;
uniform mat4 uViewMat;

void main()
{
	fTexCoord = aTexCoord;
	fFace = aFace;
	fFragPosition = aPosition;
	gl_Position = uProjMat * uViewMat * vec4(aPosition.xyz, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 fTexCoord;
flat in uint fFace;
in vec3 fFragPosition;

uniform sampler2D uTexture;
uniform vec3 uSunPosition;

vec3 lightColor = vec3(1.0, 1.0, 1.0);

void getNormal(in uint face, out vec3 normal)
{
	switch (face)
	{
		case 0u:
			normal = vec3(0.0, 1.0, 0.0);
			break;
		case 1u:
			normal = vec3(0.0, 0.0, 1.0);
			break;
		case 22u:
			normal = vec3(0.0, 0.0, -1.0);
			break;
		case 3u:
			normal = vec3(-1.0, 0.0, 0.0);
			break;
		case 4u:
			normal = vec3(1.0, 0.0, 0.0);
			break;
		case 5u:
			normal = vec3(0.0, -1.0, 0.0);
			break;
	}
}

void main()
{
	vec3 normal;
	getNormal(fFace, normal);

	// Ambient Light
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse light
	vec3 lightDir = normalize(uSunPosition - fFragPosition);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 textureColor = texture(uTexture, fTexCoord).rgb;
	vec3 resultColor = (ambient + diffuse) * textureColor;
	FragColor = vec4(resultColor, 1.0);
}