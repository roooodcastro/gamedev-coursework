#include "Entity.h"


Entity::Entity(void) {
	position = new Vector3();
	customShader = NULL;
}

Entity::Entity(Vector3 &position, Vector3 &velocity, Vector3 &rotation) {
	this->position = &position;
	this->velocity = &velocity;
	this->rotation = &rotation;
	customShader = NULL;
}

Entity::~Entity(void) {
	delete position;
	delete velocity;
	delete rotation;
}

void Entity::draw(unsigned int millisElapsed) {
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