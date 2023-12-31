#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ECS.h"

enum class Direction {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
/*
class Camera
{
private:
	EntityID m_cameraID;

public:
	Camera();

	glm::mat4 GetViewMatrix();
	//glm::mat4 GetProjectionMatrix();

	EntityID GetCameraID() const { return m_cameraID; }
};
*/
class Camera
{
private:
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	EntityID m_cameraID;

	float m_yaw;
	float m_pitch;

	float m_movementSpeed;
	float m_mouseSensitivity;

public:
	glm::vec3 m_cameraPos;
	Camera();
	//Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Direction direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

	EntityID GetCameraID() const { return m_cameraID; }

private:
	void UpdateCameraVectors();
};