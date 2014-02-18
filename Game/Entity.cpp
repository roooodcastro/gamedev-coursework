#include "Entity.h"

Entity::Entity(void) {
	position = new Vector3(0, 0, 0);
	velocity = new Vector3(0, 0, 0);
	rotation = new Vector3(0, 0, 0);
	scale = new Vector3(1.0f, 1.0f, 1.0f);
	modelMatrix = new Matrix4();
	customShader = NULL;
}

Entity::Entity(Vector3 &position, Vector3 &velocity, Vector3 &rotation, Vector3 &scale) {
	this->position = &position;
	this->velocity = &velocity;
	this->rotation = &rotation;
	this->scale = &scale;
	modelMatrix = new Matrix4();
	customShader = NULL;
}

Entity::~Entity(void) {
	delete position;
	delete velocity;
	delete rotation;
	delete scale;
	delete modelMatrix;
}

void Entity::calculateModelMatrix() {
	Vector3 calcPos = *(this->position);
	Vector3 calcVel = *(this->velocity);
	Vector3 calcRot = *(this->rotation);
	Vector3 calcSiz = *(this->scale);
	/*
	 * If is a child, we add up the parent's attributes.
	 *
	 * TODO: This method won't work if we have an entity that is a child of a child.
	 * In this case, the "grandson" will have its attributes relative to the first child,
	 * but the first child's attributes will be considered global, instead of relative
	 * to the parent's.
	 * Example: e3 is child of e2. e2 is child of e1.
	 * e1 pos: 10, 0, 0
	 * e2 pos: -2, 0, 0
	 * e3 pos: 5, 0, 0
	 * e3 final position will erroneous be 3, 0, 0, because the function doesn't check
	 * if the parent has a parent, and so on.
	 * In the unlikely event that my game has such complex entity relationship,
	 * I may or may not try to fix this.
	 */
	if (parent) {
		calcPos = calcPos + *(parent->position);
		calcVel = calcVel + *(parent->velocity);
		calcRot = calcRot + *(parent->rotation);
		calcSiz = calcSiz * *(parent->scale);
	}

	// Now that we calculated the final attributes, build the matrix
	Matrix4 rotationMatrix = Matrix4::Rotation(calcRot.x, Vector3(1, 0, 0)) * Matrix4::Rotation(calcRot.y, Vector3(0, 1, 0)) * Matrix4::Rotation(calcRot.z, Vector3(0, 0, 1));
	modelMatrix = &(Matrix4::Translation(calcPos) * rotationMatrix * Matrix4::Scale(calcSiz));

	// Finally, do the same for the children
	for(std::vector<Entity*>::iterator it = childEntities.begin(); it != childEntities.end(); ++it) {
		// Bad things will happen if two entities are parent and child of each other at the same time, please don't do that.
		(*it)->calculateModelMatrix();
	}
}

void Entity::update(unsigned millisElapsed) {
	// Apply all logic necessary

	// TODO: apply some logic

	/*
	 * Then we update the model matrix, only if this entity is not a child.
	 * This is because this function also update all children's matrices.
	 */
	if (!parent) {
		calculateModelMatrix();
	}
}

void Entity::draw(unsigned millisElapsed) {
	if (model) {
		model->draw();
	}
}

void Entity::addChild(Entity *child) {
	// TODO
}

void Entity::removeChild(Entity *child) {
	// TODO
}

void Entity::makeOrphan() {
	if (parent) {
		parent->removeChild(this);
	}
}