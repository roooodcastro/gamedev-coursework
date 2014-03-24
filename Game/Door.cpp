#include "Door.h"

const float Door::CLOSING_TIME = 2000.0f;

Door::Door(void) : Entity() {
	this->model = Model::getOrCreate("TRACK_DOOR_MESH", "resources/models/Door.mdl");
	this->targetOpenness = 1.0f;
	this->openness = 1.0f;
	this->active = false;
	this->doorIndex = 0;
	this->physicalBody->setMass(10.0f);
	this->physicalBody->setCollisionGroup(1);
	// Create all of the collision bodies
	// This creates a pyramid of small spheres, covering the whole door
	for (int i = 0; i < 1; i++) {
		// Bodies in this layer: i + 1
		for (int j = 0; j < (i + 1); j++) {
			float posX = (0.2f * j) - (0.1f * i);
			float posY = 0.8f - (0.173f * i);
			physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(posX, posY, 0), 0.1f));
		}
	}
}

Door::Door(DoorSet *doorSet, int index, float targetOpenness) : Entity() {
	this->model = Model::getOrCreate("TRACK_DOOR_MESH", "resources/models/Door.mdl");
	this->targetOpenness = targetOpenness;
	this->openness = 1.0f;
	this->active = false;
	this->doorIndex = index;
	this->doorSet = doorSet;
	this->getPhysicalBody()->setRotation(Vector3(0, 0, 60.0f * index));
	this->getPhysicalBody()->setScale(Vector3(0.58f, 0.58f, 0.193f));
	this->physicalBody->setMass(10.0f);
	this->physicalBody->setCollisionGroup(1);
	physicalBody->setCanMove(false);
	setInitialDoorPosition();
	// Create all of the collision bodies
	// This creates a pyramid of small spheres, covering the whole door
	for (int i = 0; i < 6; i++) {
		// Bodies in this layer: i + 1
		for (int j = 0; j <= i; j++) {
			float posX = (3.6f * j) - (1.8f * i);
			float posY = 13.6 - (3.0f * i);
			physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(posX, posY, 0), 2.1f));
		}
	}
	// Add a top collision sphere, so the ship doesn't clip the tip
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(0, 16.0f, 0), 1.2f));
}

Door::Door(const Door &copy) : Entity(copy) {
	this->targetOpenness = copy.targetOpenness;
	this->openness = copy.openness;
	this->active = active;
	this->doorIndex = copy.doorIndex;
}

Door::~Door(void) {}

Door &Door::operator=(const Door &other) {
	this->targetOpenness = other.targetOpenness;
	this->openness = other.openness;
	this->active = other.active;
	this->doorIndex = other.doorIndex;
	return *this;
}

void Door::update(unsigned millisElapsed) {
	// The direction vector that the door will move along
	Vector3 direction = Vector3(-sin((60.0f * PI / 180) * doorIndex), cos((60.0f * PI / 180) * doorIndex), 0);
	// Set velocity values in case door is active or not
	if (active) {
		physicalBody->setVelocity(direction * 15.0f, (float) millisElapsed);
	} else {
		physicalBody->setVelocity(Vector3(0, 0, 0), millisElapsed);
	}
	// The distance from the door to the center of the doorset
	float distance = sqrt((pow(physicalBody->getPosition()->x, 2)) + (pow(physicalBody->getPosition()->y, 2)));
	// 17.4f is the distance from the center of the doorset in which a door will be fully closed,
	// and 43.4f is the distance in which the door will be completely opened. The distance between
	// these distances is 26
	openness = (distance - 17.4f) / 26.0f;

	if (openness <= targetOpenness && (targetOpenness - openness) < 0.01f) {
		openness = targetOpenness;
		active = false;
		physicalBody->setVelocity(Vector3(0, 0, 0), (float) millisElapsed);
	} else if (openness < targetOpenness) {
		physicalBody->setVelocity(direction * -5.0f, (float) millisElapsed);
	}
	Entity::update(millisElapsed);
}

void Door::onKeyUp(SDL_Keysym key) {
	if (key.sym == SDLK_o) {
		active = true;
	}
	if (key.sym == SDLK_r) {
		openness = 1.0;
		setInitialDoorPosition();
	}
}

void Door::setInitialDoorPosition() {
	float sinX = sin((60.0f * PI / 180) * doorIndex);
	float cosY = cos((60.0f * PI / 180) * doorIndex);
	physicalBody->setPosition(Vector3(43.4f * sinX, -43.4f * cosY, -45.0f + doorSet->getPosition()));
	physicalBody->setVelocity(Vector3(0, 0, 0), 1);
}