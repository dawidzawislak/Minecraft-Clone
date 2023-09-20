#include "Camera.h"

#include <iostream>

Camera::Camera()
{
    m_cameraID = EntityRegistry::NewEntity();
    EntityRegistry::AssignComponent<Transform>(m_cameraID);
}

glm::mat4 Camera::GetViewMatrix()
{
    Transform* transform = EntityRegistry::GetComponent<Transform>(m_cameraID);

    return glm::lookAt(transform->position, transform->position + transform->front, transform->up);
}