#include "Ship.h"

const float Ship::MAX_SIDE_SPEED = 40.0f;

Ship::Ship(void) : Entity() {
	model = Model::getOrCreate("SHIP_MESH", "resources/models/testShip.mdl");
	roll = 0;
	pitch = 0;
	yaw = 0;
	physicalBody->setMass(100.0f);
	physicalBody->setDragFactor(0.2f);
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
	physicalBody->setMass(100.0f);
	physicalBody->setDragFactor(0.2f);
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
	Vector3 currentVel = physicalBody->getVelocity(millisElapsed);
	physicalBody->setForce(Vector3());
	// Controls yaw
	if (keyboard->isKeyPressed(SDLK_a)) {
		yaw += 2;
		if (yaw > 15) {
			yaw = 15;
		}
		physicalBody->addForce(Vector3((MAX_SIDE_SPEED - currentVel.x) * 50.0f, 0, 0));
	} else if (keyboard->isKeyPressed(SDLK_d)) {
		yaw -= 2;
		if (yaw < -15) {
			yaw = -15;
		}
		physicalBody->addForce(Vector3((-MAX_SIDE_SPEED - currentVel.x) * 50.0f, 0, 0));
	} else if (abs(yaw) > 0) {
		yaw /= 1.1f;
		if (abs(yaw) < 1.0f ) {
			yaw = 0;
		}
	}

	// Controls roll
	if (keyboard->isKeyPressed(SDLK_q)) {
		roll -= 9;
		if (roll < -90) {
			roll = -90;
		}
	} else if (keyboard->isKeyPressed(SDLK_e)) {
		roll += 9;
		if (roll > 90) {
			roll = 90;
		}
	} else if (abs(roll) > 0) {
		roll /= 1.3f;
		if (abs(roll) < 1.0f ) {
			roll = 0;
		}
	}

	// Controls pitch
	if (keyboard->isKeyPressed(SDLK_w)) {
		pitch += 2;
		if (pitch > 15) {
			pitch = 15;
		}
		physicalBody->addForce(Vector3(0, (-MAX_SIDE_SPEED - currentVel.y) * 50.0f, 0));
	} else if (keyboard->isKeyPressed(SDLK_s)) {
		pitch -= 2;
		if (pitch < -15) {
			pitch = -15;
		}
		physicalBody->addForce(Vector3(0, (MAX_SIDE_SPEED - currentVel.y) * 50.0f, 0));
	} else if (abs(pitch) > 0) {
		pitch /= 1.1f;
		if (abs(pitch) < 1.0f ) {
			pitch = 0;
		}
	}

	this->getPhysicalBody()->setRotation(Vector3(pitch, yaw, roll));
	Entity::update(millisElapsed);
	level->setCameraRotation(Vector3((pitch / 1.2f) + 10, 180 - (yaw / 1.2f), -roll / 2.0f));
	level->setCameraPosition(*(this->getPhysicalBody()->getPosition()));
	//velocity->x /= 1.1f;
	//velocity->y /= 1.1f;
	/*if (abs(velocity->x) < 0.5f) {
		velocity->x = 0.0f;
	}
	if (abs(velocity->y) < 0.5f) {
		velocity->y = 0.0f;
	}*/
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