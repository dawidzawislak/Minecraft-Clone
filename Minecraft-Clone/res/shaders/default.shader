#shader vertex
#version 330 core

layout(location = 0) in uint aData1;
layout(location = 1) in uint aData2;

out vec2 fTexCoord;
flat out uint fFace;
out vec3 fFragPosition;
out float visibility;

uniform mat4 uProjMat;
uniform mat4 uViewMat;
uniform ivec2 uChunkPosition;
uniform samplerBuffer uUVsTexture;

#define MASK_POSITION 0x1FFFF
#define MASK_FACE 0xE0000
#define MASK_TEXID 0xFFF00000

// First 17 bits - position is save as 17 base 4 digit number (x[0-16],2y[0-288], z[0-16])
// Bits 18-20 - block face [0-5]
// Bits 21-32 - textureID
void unpackData(in uint data1, in uint data2, out uvec3 position, out uint face, out uint textureID, out uint vertexPos)
{
	uint dataPos = data1 & uint(MASK_POSITION);
	position.x = dataPos % 17u;
	position.y = (dataPos / 17u) % (289u);
	position.z = (dataPos / (4913u)) % 17u;

	uint dataFace = data1 & uint(MASK_FACE);
	face = (dataFace >> 17) % 6u;

	uint dataTexID = data1 & uint(MASK_TEXID);
	textureID = (dataTexID >> 20);

	vertexPos = data2 % 4u;
}

const float density = 0.002;
const float gradient = 100.0;

void main()
{
	uvec3 localPosition;
	uint vertexPos;
	uint textureID;
	unpackData(aData1, aData2, localPosition, fFace, textureID, vertexPos);

	vec3 globalPosition = localPosition + vec3(uChunkPosition.x * 16, 0.0, uChunkPosition.y * 16);

	// Get UV from UVTextureBuffer
	uint index = (8u * textureID) + (2u * vertexPos);
	fTexCoord.x = texelFetch(uUVsTexture, int(index)).r;
	fTexCoord.y = texelFetch(uUVsTexture, int(index + 1u)).r;

	fFragPosition = globalPosition;

	float distance = length(globalPosition);
	visibility = exp(-pow(distance * density, gradient));
	visibility = clamp(visibility, 0.0, 1.0);

	gl_Position = uProjMat * uViewMat * vec4(globalPosition, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 fTexCoord;
flat in uint fFace;
in vec3 fFragPosition;
in float visibility;

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
		case 2u:
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
	FragColor = mix(vec4(0.2, 0.3, 0.3, 1.0), vec4(resultColor, 1.0), visibility);
}