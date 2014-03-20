#include "Ship.h"

const float Ship::MAX_SIDE_SPEED = 80.0f;

Ship::Ship(void) : Entity() {
	model = Model::getOrCreate("SHIP_MESH", "resources/models/testShip.mdl");
	roll = 0;
	pitch = 0;
	yaw = 0;
	physicalBody->setMass(10.0f);
	physicalBody->setDragFactor(0.3f);
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
	physicalBody->setDragFactor(0.3f);
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
		yaw += 1.0f;
		if (yaw > 10) yaw = 10;
		force->x = (currentVel.x <= MAX_SIDE_SPEED) ? 250.0f : 0.0f;
	} else if (keyboard->isKeyPressed(SDLK_d)) {
		yaw -= 1.0f;
		if (yaw < -10) yaw = -10;
		force->x = (currentVel.x >= -MAX_SIDE_SPEED) ? -250.0f : 0.0f;
	} else {
		yaw = (abs(yaw) > 0.1f) ? (yaw / 1.1f) : 0;
		force->x = -currentVel.x * 10.0f;
	}

	// Controls pitch (up-down movement)
	if (keyboard->isKeyPressed(SDLK_w)) {
		pitch += 1.0f;
		if (pitch > 10) pitch = 10;
		force->y = (currentVel.y >= -MAX_SIDE_SPEED) ? -250.0f : 0.0f;
	} else if (keyboard->isKeyPressed(SDLK_s)) {
		pitch -= 1.0f;
		if (pitch < -10) pitch = -10;
		force->y = (currentVel.y <= MAX_SIDE_SPEED) ? 250.0f : 0.0f;
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

	if (keyboard->isKeyPressed(SDLK_UP)) {
		force->z = (currentVel.z <= 200) ? 500.0f : 0.0f;
	} else if (keyboard->isKeyPressed(SDLK_DOWN)) {
		force->z = (currentVel.z >= -200) ? -500.0f : 0.0f;
	} else {
		force->z = 0;
	}

	this->getPhysicalBody()->setRotation(Vector3(pitch, yaw, roll));
	Entity::update(millisElapsed);
	level->setCameraRotation(Vector3((pitch / 1.5f) + 10, 180 - (yaw / 1.5f), -roll / 2.0f));
	Vector3 position = Vector3(*(this->getPhysicalBody()->getPosition()));
	level->setCameraPosition(position);

	// Adjust light sources position to follow the ship
	std::vector<Light*> *lightSources = level->getLightSources();
	for (auto it = lightSources->begin(); it != lightSources->end(); it++) {
		(*it)->position.z = position.z - 10.0f;
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