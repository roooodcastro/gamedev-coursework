#include "Door.h"

const float Door::CLOSING_TIME = 2000.0f;

Door::Door(void) : Entity() {
	this->model = Model::loadObjFile("resources/models/Door.mdl");
	this->targetOpenness = 100;
	this->openness = 100;
	this->active = false;
	this->doorIndex = 0;
}

Door::Door(DoorSet *doorSet, int index, float targetOpenness) : Entity() {
	this->model = Model::loadObjFile("resources/models/Door.mdl");
	this->targetOpenness = targetOpenness;
	this->openness = 100;
	this->active = false;
	this->doorIndex = index;
	this->doorSet = doorSet;
	// TODO: Improve the position calculation, making it automatic and getting rid of the switch
	this->setRotation(Vector3(0, -90, 60 * index));
	this->setScale(Vector3(0.193f, 0.58f, 0.58f));
	updateDoorPosition();
}

Door::Door(const Door &copy) : Entity(copy) {
	this->targetOpenness = copy.targetOpenness;
	this->openness = copy.openness;
	this->active = active;
	this->doorIndex = copy.doorIndex;
}

Door::~Door(void) {
	
}

Door &Door::operator=(const Door &other) {
		
	this->targetOpenness = other.targetOpenness;
	this->openness = other.openness;
	this->active = other.active;
	this->doorIndex = other.doorIndex;
	return *this;
}

void Door::update(unsigned millisElapsed) {
	if (openness > targetOpenness && active) {
		// If it's more opened than it should be, close it
		openness -= (millisElapsed / CLOSING_TIME) * 100;
		if (openness <= targetOpenness) {
			openness = targetOpenness;
			active = false;
		}
		updateDoorPosition();
	} else if (openness <= targetOpenness && active) {
		active = false;
	}
	Entity::update(millisElapsed);
}

void Door::onKeyUp(SDL_Keysym key) {
	if (key.sym == SDLK_o) {
		active = true;
	}
	if (key.sym == SDLK_r) {
		openness = 100;
		updateDoorPosition();
	}
}

void Door::updateDoorPosition() {
	float factor = (openness / 100.0f);
	switch (doorIndex) {
	case 0:
		this->setPosition(Vector3(0, -17.4f - factor * 26.2f, -45.0f + doorSet->getPosition()));
		break;
	case 1:
		this->setPosition(Vector3(15.0f + factor * 22.69f, -8.68f - factor * 13.1f, -45.0f + doorSet->getPosition()));
		break;
	case 2:
		this->setPosition(Vector3(15.0f + factor * 22.69f, 8.68f + factor * 13.1f, -45.0f + doorSet->getPosition()));
		break;
	case 3:
		this->setPosition(Vector3(0, 17.4f + (openness / 100.0f) * 26.2f, -45.0f + doorSet->getPosition()));
		break;
	case 4:
		this->setPosition(Vector3(-15.0f - factor * 22.69f, 8.68f + factor * 13.1f, -45.0f + doorSet->getPosition()));
		break;
	case 5:
		this->setPosition(Vector3(-15.0f - factor * 22.69f, -8.68f - factor * 13.1f, -45.0f + doorSet->getPosition()));
		break;
	}
}