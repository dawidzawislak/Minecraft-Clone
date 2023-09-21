#pragma once

#include "ECS.h"

class Player
{
private:
	EntityID m_playerID;
	float m_movementSpeed;
	float m_mouseSensitivity;

	float m_elapsedSinceJump;
	float m_prevY;

public:
	Player(glm::vec3 pos);

	void Update(float dt);

	EntityID GetPlayerID() const { return m_playerID; }

private:
	void UpdateTransformVectors(Transform* transform);
};