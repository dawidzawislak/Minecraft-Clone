#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	:m_cameraPos(position), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_up(up), m_right(glm::vec3(1.0f, 0.0f, 0.0f)), m_worldUp(up), 
	 m_yaw(yaw), m_pitch(pitch), m_movementSpeed(3.0f), m_mouseSensitivity(0.1f)
{
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_cameraPos, m_cameraPos + m_front, m_up);
}

void Camera::ProcessKeyboard(Direction direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;

    if (direction == Direction::FORWARD)
        m_cameraPos += m_front * velocity;
    if (direction == Direction::BACKWARD)
        m_cameraPos -= m_front * velocity;
    if (direction == Direction::LEFT)
        m_cameraPos -= m_right * velocity;
    if (direction == Direction::RIGHT)
        m_cameraPos += m_right * velocity;

    if (direction == Direction::UP)
        m_cameraPos += m_up * velocity;
    if (direction == Direction::DOWN)
        m_cameraPos -= m_up * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}