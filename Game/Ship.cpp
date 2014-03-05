#include "Ship.h"

const float Ship::MAX_SIDE_SPEED = 40.0f;

Ship::Ship(void) : Entity() {
	model = Model::loadObjFile("resources/models/testShip.obj");
	roll = 0;
	pitch = 0;
	yaw = 0;
}

Ship::Ship(const Ship &copy) : Entity(copy) {

}

Ship::Ship(Vector3 &position, Vector3 &velocity, Vector3 &rotation) : Entity(position, velocity, rotation, Vector3(1, 1, 1)) {
	model = Model::loadObjFile("resources/models/testShip.obj");
	roll = 0;
	pitch = 0;
	yaw = 0;
}

Ship::~Ship(void) {

}

Ship &Ship::operator=(const Ship &other) {
	static_cast<Entity&>(*this) = other;

	return *this;
}

void Ship::update(unsigned millisElapsed) {
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	Level *level = GameApp::getInstance()->getCurrentLevel();

	// Controls yaw
	if (keyState[SDL_SCANCODE_A]) {
		yaw += 2;
		if (yaw > 15) {
			yaw = 15;
		}
		velocity->x = MAX_SIDE_SPEED;
	} else if (keyState[SDL_SCANCODE_D]) {
		yaw -= 2;
		if (yaw < -15) {
			yaw = -15;
		}
		velocity->x = -MAX_SIDE_SPEED;
	} else if (abs(yaw) > 0) {
		yaw /= 1.1f;
		if (abs(yaw) < 1.0f ) {
			yaw = 0;
		}
	}

	// Controls roll
	if (keyState[SDL_SCANCODE_Q]) {
		roll -= 9;
		if (roll < -90) {
			roll = -90;
		}
	} else if (keyState[SDL_SCANCODE_E]) {
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
	if (keyState[SDL_SCANCODE_W]) {
		pitch += 2;
		if (pitch > 15) {
			pitch = 15;
		}
		velocity->y = -MAX_SIDE_SPEED;
	} else if (keyState[SDL_SCANCODE_S]) {
		pitch -= 2;
		if (pitch < -15) {
			pitch = -15;
		}
		velocity->y = MAX_SIDE_SPEED;
	} else if (abs(pitch) > 0) {
		pitch /= 1.1f;
		if (abs(pitch) < 1.0f ) {
			pitch = 0;
		}
	}

	this->rotation->x = pitch;
	this->rotation->y = yaw;
	this->rotation->z = roll;
	Entity::update(millisElapsed);
	level->setCameraRotation(Vector3((pitch / 1.2f) + 10, 180 - (yaw / 1.2f), -roll / 2.0f));
	level->setCameraPosition(*(this->position));
	velocity->x /= 1.1f;
	velocity->y /= 1.1f;
	if (abs(velocity->x) < 0.5f) {
		velocity->x = 0.0f;
	}
	if (abs(velocity->y) < 0.5f) {
		velocity->y = 0.0f;
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