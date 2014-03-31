#include "Ship.h"

float Ship::maxSideSpeed = 50.0f;

Ship::Ship(void) : Entity() {
	model = Model::getOrCreate("SHIP_MESH", "resources/models/testShip.mdl");
	roll = 0;
	pitch = 0;
	yaw = 0;
	physicalBody->setMass(10.0f);
	physicalBody->setDragFactor(0.0f);
	physicalBody->setElasticity(0.0f);
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(-1.2f, 0.2f, -1.0f), 0.5f));
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(0.0f, 0.2f, -1.0f), 0.5f));
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(1.2f, 0.2f, -1.0f), 0.5f));
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(0.0f, 0.6f, 0.5f), 0.5f));
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(0.0f, 0.5f, 2.0f), 0.5f));
}

Ship::Ship(const Ship &copy) : Entity(copy) {
	this->pitch = copy.pitch;
	this->roll = copy.roll;
	this->yaw = copy.yaw;
}

Ship::Ship(Vector3 &position, Vector3 &velocity, Vector3 &rotation) : Entity(position, velocity, rotation, Vector3(1, 1, 1)) {
	model = Model::getOrCreate("SHIP_MESH", "resources/models/testShip.mdl");
	roll = 0;
	pitch = 0;
	yaw = 0;
	physicalBody->setMass(10.0f);
	physicalBody->setDragFactor(0.0f);
	physicalBody->setElasticity(0.0f);
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(-1.2f, 0.2f, -1.0f), 0.5f));
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(0.0f, 0.2f, -1.0f), 0.5f));
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(1.2f, 0.2f, -1.0f), 0.5f));
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(0.0f, 0.6f, 0.5f), 0.5f));
	physicalBody->addCollisionBody(new CollisionBody(physicalBody, Vector3(0.0f, 0.2f, 2.0f), 0.5f));
}

Ship::~Ship(void) {}

Ship &Ship::operator=(const Ship &other) {
	static_cast<Entity&>(*this) = other;
	this->pitch = other.pitch;
	this->roll = other.roll;
	this->yaw = other.yaw;
	return *this;
}

void Ship::update(unsigned millisElapsed) {
	Level *level = GameApp::getInstance()->getCurrentLevel();
	Keyboard *keyboard = Keyboard::getInstance();
	Vector3 currentVel = physicalBody->getVelocity((float) millisElapsed);
	Vector3 *force = physicalBody->getForce();

	// Controls yaw (left-right movement)
	if (keyboard->isKeyPressed(SDLK_a)) {
		yaw += 2.0f;
		if (yaw > 10) yaw = 10;
		physicalBody->setAcceleration(Vector3((maxSideSpeed * (1.0f + physicalBody->getdragFactor())) - currentVel.x, 0, 0));
	} else if (keyboard->isKeyPressed(SDLK_d)) {
		yaw -= 2.0f;
		if (yaw < -10) yaw = -10;
		physicalBody->setAcceleration(Vector3((-maxSideSpeed * (1.0f + physicalBody->getdragFactor())) - currentVel.x, 0, 0));
	} else {
		yaw = (abs(yaw) > 0.1f) ? (yaw / 1.1f) : 0;
		physicalBody->setAcceleration(Vector3(currentVel.x * -1.0f, 0, 0));
	}

	// Controls pitch (up-down movement)
	if (keyboard->isKeyPressed(SDLK_w)) {
		pitch += 2.0f;
		if (pitch > 10) pitch = 10;
		force->y = (currentVel.y >= -maxSideSpeed) ? -500.0f : 0.0f;
	} else if (keyboard->isKeyPressed(SDLK_s)) {
		pitch -= 2.0f;
		if (pitch < -10) pitch = -10;
		force->y = (currentVel.y <= maxSideSpeed) ? 500.0f : 0.0f;
	} else {
		pitch = (abs(pitch) > 0.1f) ? (pitch / 1.1f) : 0;
		force->y = -currentVel.y * 10.0f;
	}

	// Controls roll (roll 60 degrees left-right)
	if (keyboard->isKeyPressed(SDLK_q)) {
		roll -= 6;
		if (roll < -60) roll = -60;
	} else if (keyboard->isKeyPressed(SDLK_e)) {
		roll += 6;
		if (roll > 60) roll = 60;
	} else {
		roll = (abs(roll) > 0.1f) ? (roll / 1.1f) : 0;
	}

	Vector3 distanceFromCenter = *(physicalBody->getPosition()) - Vector3(0, 0, physicalBody->getPosition()->z);
	if (distanceFromCenter.getLength() > 24.0f) {
		Vector3 correction = ((distanceFromCenter - (distanceFromCenter.normalised() * 23.0f)) * -1.0f);
		Vector3 correction2 = ((distanceFromCenter - (distanceFromCenter.normalised() * 24.0f)) * -1.0f);
		correction.x = pow(correction.x, 3);
		correction.y = pow(correction.y, 3);
		//physicalBody->setAcceleration(correction);
	}

	this->getPhysicalBody()->setRotation(Vector3(pitch, yaw, roll));
	Entity::update(millisElapsed);
	level->setCameraRotation(Vector3((pitch / 1.5f) + 5, 180 - (yaw / 1.5f), -roll / 2.0f));
	Vector3 position = Vector3(*(this->getPhysicalBody()->getPosition()));
	level->setCameraPosition(position);

	// Adjust light sources position to follow the ship
	std::vector<Light*> *lightSources = level->getLightSources();
	for (auto it = lightSources->begin(); it != lightSources->end(); it++) {
		(*it)->position.z = position.z - 5.0f;
	}
}

void Ship::draw(unsigned millisElapsed) {
	Entity::draw(millisElapsed);
}

void Ship::onKeyPress(SDL_Keysym key) {
}

void Ship::onKeyDown(SDL_Keysym key) {
}

void Ship::onKeyUp(SDL_Keysym key) {
}