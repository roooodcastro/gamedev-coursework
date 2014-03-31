#pragma once

#include "Entity.h"
#include "ResourcesManager.h"

class Ship : public Entity {

public:
	Ship(void);
	Ship(const Ship &copy);
	Ship(Vector3 &position, Vector3 &velocity, Vector3 &rotation);
	virtual ~Ship(void);

	Ship &operator=(const Ship &other);

	virtual void update(unsigned millisElapsed);
	virtual void draw(unsigned millisElapsed);

	/* Mouse events */
	//virtual void onMouseMoved(Vector2 &position, Vector2 &amount) {} // Will fire every time the mouse moves
	//virtual void onMouseClick(Uint8 button, Vector2 &position) {} // Will fire once a mouse button is released
	//virtual void onMouseDoubleClick(Uint8 button, Vector2 &position) {} // Will fire on a double click
	//virtual void onMouseButtonDown(Uint8 button, Vector2 &position) {} // Will fire in every tick that a button is down
	//virtual void onMouseButtonUp(Uint8 button, Vector2 &position) {} // Will fire every time a mouse button is released
	//virtual void onMouseWheelScroll(int amount) {} // Will fire every time the mouse wheel scrolls
	/* Keyboard events */
	virtual void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	virtual void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
	virtual void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

	static void setSideSpeed(float speed) { Ship::maxSideSpeed = speed; }
	static float getSideSpeed() { return Ship::maxSideSpeed; }

	/* The speed in which the ship will be able to move left, right, up and down */
	static float maxSideSpeed;

protected:

	/* Angle measurements for the ship movement */
	float roll;
	float pitch;
	float yaw;

};

