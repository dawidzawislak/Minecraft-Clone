#pragma once

#include "ECS.h"
#include "ChunkLoader.h"
#include <set>
#include "Input.h"
#include <glm/gtx/vector_angle.hpp>

std::vector<glm::vec3> Game::boxes;

class Physics
{
	static float m_gravity;
	static bool m_playerOnGround;
	static bool m_firtTimeOnGround;
	static bool m_blockedX, m_blockedY;
	static glm::ivec3 m_prevPos;

public:
	static void Update(float dt)
	{


		for (EntityID id : RegistryView<Transform, RigidBody>())
		{
			RigidBody* rb = EntityRegistry::GetComponent<RigidBody>(id);
			if (!m_playerOnGround)
				rb->acceleration.y = -m_gravity;
			rb->velocity.y += rb->acceleration.y * dt;
			Transform* t = EntityRegistry::GetComponent<Transform>(id);
			t->position += rb->velocity * dt;
		}

		// Resolve collisions
		for (EntityID id : RegistryView<Transform, BoxCollider, RigidBody>())
		{
			Game::boxes.clear();

			Transform* t = EntityRegistry::GetComponent<Transform>(id);
			BoxCollider* bc = EntityRegistry::GetComponent<BoxCollider>(id);

			BoxCollider boxCollider;
			boxCollider.size = glm::vec3(0.5f);

			Transform boxTransform;

			RigidBody* rb = EntityRegistry::GetComponent<RigidBody>(id);

			Transform newTransform;
			newTransform.position = t->position;
			//newTransform.position += rb->velocity * dt;

			std::vector<glm::ivec3> toCheckY;
			glm::vec3 leftDownDown = newTransform.position - bc->size;
			glm::vec3 rightUpUp = newTransform.position + bc->size;
			std::vector<glm::ivec3> toCheckXZ;
			glm::vec3 leftUpUp = rightUpUp;
			leftUpUp.x = leftDownDown.x;
			glm::vec3 rightDownDown = leftDownDown;
			rightDownDown.x = rightUpUp.x;

			// XZ Axis
			toCheckXZ.emplace_back(floor(leftDownDown.x), ceil(leftDownDown.y), floor(leftDownDown.z));
			toCheckXZ.emplace_back(floor(leftDownDown.x), ceil(leftDownDown.y), floor(leftUpUp.z));
			toCheckXZ.emplace_back(floor(leftDownDown.x), floor(leftUpUp.y), floor(leftUpUp.z));
			toCheckXZ.emplace_back(floor(leftDownDown.x), floor(leftUpUp.y), floor(leftUpUp.z));
			toCheckXZ.emplace_back(floor(rightDownDown.x), ceil(rightDownDown.y), floor(rightDownDown.z));
			toCheckXZ.emplace_back(floor(rightDownDown.x), ceil(rightDownDown.y), floor(rightUpUp.z));
			toCheckXZ.emplace_back(floor(rightDownDown.x), floor(rightUpUp.y), floor(rightDownDown.z));
			toCheckXZ.emplace_back(floor(rightDownDown.x), floor(rightUpUp.y), floor(rightUpUp.z));

			float margin = 0.00001f;

			for (const auto& blockCoords : toCheckXZ) {
				if (ChunkLoader::GetBlock(blockCoords) != BlockType::AIR) {
					boxTransform.position = glm::vec3(blockCoords) + boxCollider.size;

					glm::vec3 intersect = GetIntersection(t, bc, &boxTransform, &boxCollider);

					if (abs(intersect.x) < abs(intersect.z)) {
						if (abs(intersect.x) > 0.0f) {
							rb->velocity.x = 0.0f;
							m_blockedX = true;
							if (intersect.x > 0)
								intersect.x += margin;
							else
								intersect.x -= margin;
							t->position.x += intersect.x;
						}
					}
					else {
						rb->velocity.z = 0.0f;
						m_blockedY = true;
						if (intersect.z > 0)
							intersect.z += margin;
						else
							intersect.z -= margin;
						t->position.z += intersect.z;
					}
				}
			}
		}


		// Y Axis
		for (EntityID id : RegistryView<Transform, BoxCollider, RigidBody>())
		{
			Transform* t = EntityRegistry::GetComponent<Transform>(id);
			BoxCollider* bc = EntityRegistry::GetComponent<BoxCollider>(id);

			BoxCollider boxCollider;
			boxCollider.size = glm::vec3(0.5f);

			Transform boxTransform;

			RigidBody* rb = EntityRegistry::GetComponent<RigidBody>(id);

			std::vector<glm::ivec3> toCheckY;
			glm::vec3 leftDownDown = t->position - bc->size;
			glm::vec3 rightUpUp = t->position + bc->size;
			glm::vec3 leftUpUp = rightUpUp;
			leftUpUp.x = leftDownDown.x;
			glm::vec3 rightDownDown = leftDownDown;
			rightDownDown.x = rightUpUp.x;

			// Y Axis
			//m_playerOnGround = true;
			//rb->velocity.y = 0;
			//rb->acceleration.y = 0;
			//t->position.y = 150.0f;

			leftDownDown = t->position - bc->size;
			rightUpUp = t->position + bc->size;

			toCheckY.emplace_back(floor(leftDownDown.x), floor(leftDownDown.y), floor(leftDownDown.z));
			toCheckY.emplace_back(floor(rightUpUp.x), floor(leftDownDown.y), floor(leftDownDown.z));
			toCheckY.emplace_back(floor(rightUpUp.x), floor(leftDownDown.y), floor(rightUpUp.z));
			toCheckY.emplace_back(floor(leftDownDown.x), floor(leftDownDown.y), floor(rightUpUp.z));

			bool collided = false;

			for (const auto& blockCoords : toCheckY) {
				if (ChunkLoader::GetBlock(blockCoords) != BlockType::AIR) {
					rb->velocity.y = 0;
					rb->acceleration.y = 0;
					t->position.y = blockCoords.y + 2.0f;
					m_playerOnGround = true;
					m_firtTimeOnGround = false;
					collided = true;
				}
			}

			if (m_prevPos != glm::ivec3(floor(t->position.x), floor(t->position.y), floor(t->position.z)) && !collided)
				m_playerOnGround = false;
		}
	}
private:
	Physics() {}

	static float max(float a, float b)
	{
		return (a > b ? a : b);
	}

	static float min(float a, float b)
	{
		return (a < b ? a : b);
	}

	static float GetIntervalsIntersertion(glm::vec2 a, glm::vec2 b)
	{
		if (b.x > a.y || a.x > b.y)
			return 0.0f;

		float k = 1.0f;
		if (a.x < b.x) k = -1.0f;

		return k * ( min(a.y, b.y) - max(a.x, b.x) );
	}

	static glm::vec3 GetIntersection(Transform* playerT, BoxCollider* playerBC, Transform* blockT, BoxCollider* blockBC)
	{
		glm::vec2 playerXInt(playerT->position.x - playerBC->size.x, playerT->position.x + playerBC->size.x);
		glm::vec2 boxXInt(blockT->position.x - blockBC->size.x, blockT->position.x + blockBC->size.x);

		float xInt = GetIntervalsIntersertion(playerXInt, boxXInt);

		if (xInt == 0.0f) return { 0.0f, 0.0f, 0.0f };

		glm::vec2 playerYInt(playerT->position.y - playerBC->size.y, playerT->position.y + playerBC->size.y);
		glm::vec2 boxYInt(blockT->position.y - blockBC->size.y, blockT->position.y + blockBC->size.y);

		float yInt = GetIntervalsIntersertion(playerYInt, boxYInt);

		if (yInt == 0.0f) return { 0.0f, 0.0f, 0.0f };

		glm::vec2 playerZInt(playerT->position.z - playerBC->size.z, playerT->position.z + playerBC->size.z);
		glm::vec2 boxZInt(blockT->position.z - blockBC->size.z, blockT->position.z + blockBC->size.z);

		float zInt = GetIntervalsIntersertion(playerZInt, boxZInt);

		if (zInt == 0.0f) return { 0.0f, 0.0f, 0.0f };

		return { xInt, yInt, zInt };
	}
};

float Physics::m_gravity = 30.0f;
bool Physics::m_playerOnGround = false;
glm::ivec3 Physics::m_prevPos;
bool Physics::m_firtTimeOnGround = true;
bool Physics::m_blockedX = false;
bool Physics::m_blockedY = false;