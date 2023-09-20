#pragma once

#include <glm/glm.hpp>


struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec2 orientation;

	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
};

struct BoxCollider
{
	glm::vec3 size;
};

struct RigidBody
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
};

struct Test
{
	int x;
};