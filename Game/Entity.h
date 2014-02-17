#pragma once

#include <vector>
#include "Vector3.h"
#include "Model.h"
#include "Shader.h"

class GameApp;
class Model;

class Entity {
public:
	Entity(void);
	Entity(Vector3 &position, Vector3 &velocity, Vector3 &rotation);
	~Entity(void);

	void update(unsigned int millisElapsed);
	void draw(unsigned int millisElapsed);

	void setPosition(Vector3 &newPos) { this->position = &newPos; }
	Vector3 &getPosition() { return *position; }
	void setVelocity(Vector3 &newVel) { this->velocity = &newVel; }
	Vector3 &getVelocity() { return *velocity; }
	void setRotation(Vector3 &newRot) { this->rotation = &newRot; }
	Vector3 &getRotation() { return *rotation; }

	Entity *getParent() { return parent; }

	/* Adds a new child to this entity */
	void addChild(Entity *child);

	/* Removes a child from this entity's child list */
	void removeChild(Entity *child);

	/*
	 * Makes this entity an orphan, removing it from its parent's child list, if this entity is a child.
	 * If you do this, you're a horrible person and belong in hell.
	 */
	void makeOrphan();

protected:

	/* Vectors to store this entity's position, velocity and rotation attributes */
	Vector3 *position;
	Vector3 *velocity;
	Vector3 *rotation;

	/*
	 * List of child entities for this entity. A child entity can be anything, from an attachment part to spark particles.
	 * The attributes of child entities, like position, size and rotation will be relative to its parent, instead of being
	 * relative to the world. For example, if and entity is in the position (10, 0, 0), and have a child with the position
	 * attribute set to (-2, 0, 0), the child's world position will be actually (8, 0, 0).
	 */
	std::vector<Entity*> childEntities;

	/*
	 * If this entity is the child of another entity, it must know its parent, in order to calculate its world
	 * attributes, such as position, velocity and rotation, relative to its parent's.
	 */
	Entity *parent;

	/*
	 * The 3D model for this entity. Notice this CAN be null, the entity could be just a particle, for example
	 */
	Model *model;

	/*
	 * This entity's custom shader. This is optional and will be replaced by the default shader loaded in GameApp if not specified.
	 */
	Shader *customShader;

};