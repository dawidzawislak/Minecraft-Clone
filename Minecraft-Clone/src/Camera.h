#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ECS.h"

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