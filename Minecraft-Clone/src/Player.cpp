#include "Player.h"

#include "Input.h"
#include <iostream>

Player::Player(glm::vec3 pos)
{
	m_movementSpeed = 5.0f;
	m_mouseSensitivity = 0.08f;

	m_playerID = EntityRegistry::NewEntity();
	Transform* transform = EntityRegistry::AssignComponent<Transform>(m_playerID);
	transform->position = pos;
	transform->orientation = glm::vec2(-90.0f, 0.0f);
	transform->scale = glm::vec3(1.0f);

	BoxCollider* boxCollider = EntityRegistry::AssignComponent<BoxCollider>(m_playerID);
	boxCollider->size = glm::vec3(0.4f, 1.0f, 0.4f);

	RigidBody* rb = EntityRegistry::AssignComponent<RigidBody>(m_playerID);
	rb->velocity = glm::vec3(0.0f);
	rb->acceleration = glm::vec3(0.0f);

	m_prevY = 1000.0f;

	UpdateTransformVectors(transform);
}

void Player::Update(float dt)
{
	float xOffset = m_mouseSensitivity * Input::GetMouseXOffset();
	float yOffset = m_mouseSensitivity * Input::GetMouseYOffset();

	Transform* transform = EntityRegistry::GetComponent<Transform>(m_playerID);

	transform->orientation.x += xOffset;
	transform->orientation.y += yOffset;

	if (transform->orientation.y > 89.0f)
		transform->orientation.y = 89.0f;
	else if (transform->orientation.y < -89.0f)
		transform->orientation.y = -89.0f;

	UpdateTransformVectors(transform);

	float front = 0.0f;
	float sides = 0.0f;

	RigidBody* rb = EntityRegistry::GetComponent<RigidBody>(m_playerID);

	if (Input::IsKeyPressed(GLFW_KEY_W))
		front += 1.0f;
	if (Input::IsKeyPressed(GLFW_KEY_S))
		front -= 1.0f;
	if (Input::IsKeyPressed(GLFW_KEY_A))
		sides -= 1.0f;
	if (Input::IsKeyPressed(GLFW_KEY_D))
		sides += 1.0f;

	glm::vec3 frontComp = front * glm::vec3(transform->front.x, 0.0f, transform->front.z);
	glm::vec3 sideComp = sides * transform->right;

	if (abs(front) > 0.0f)
		frontComp = glm::normalize(frontComp);

	glm::vec3 velocity = frontComp + sideComp;

	if (abs(front) > 0.0f && abs(sides) > 0.0f)
		velocity = glm::normalize(velocity);

	rb->velocity.x = m_movementSpeed * velocity.x;
	rb->velocity.z = m_movementSpeed * velocity.z;


	if (Input::IsKeyPressed(GLFW_KEY_SPACE) && transform->position.y == m_prevY)
		rb->velocity.y = 8.25f;

	m_prevY = transform->position.y;
}

void Player::UpdateTransformVectors(Transform* transform)
{
	glm::vec3 front;
	front.x = cos(glm::radians(transform->orientation.x)) * cos(glm::radians(transform->orientation.y));
	front.y = sin(glm::radians(transform->orientation.y));
	front.z = sin(glm::radians(transform->orientation.x)) * cos(glm::radians(transform->orientation.y));
	transform->front = glm::normalize(front);
	transform->right = glm::normalize(glm::cross(transform->front, glm::vec3(0.0f, 1.0f, 0.0f)));
	transform->up = glm::normalize(glm::cross(transform->right, transform->front));
}