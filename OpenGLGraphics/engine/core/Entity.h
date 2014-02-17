#pragma once
#include "../nclgl/Vector3.h"
#include "SDL.h"

class Entity
{
public:
	Entity(void);
	Entity(Vector3 &position, float aggroRange);
	~Entity(void);

	void setPosition(Vector3 &newPos) { this->position = &newPos; }
	void setAggroRange(float newAggroRange) { this->aggroRange = newAggroRange; }
	void setEntitiesInRange(int newEntitiesInRange) { this->entitiesInRange = newEntitiesInRange; }
	void addEntitiesInRange(int amount) { this->entitiesInRange += amount; }

	Vector3 &getPosition() { return *position; }
	float getAggroRange() { return aggroRange; }
	int getEntitiesInRange() { return entitiesInRange; }

	Vector3 *position;
	float aggroRange;
	int entitiesInRange;
};

