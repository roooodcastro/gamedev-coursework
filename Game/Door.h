/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is a single door that will be part of a doorset, that will be part of a
 * track piece, that will be part of the race track. A door can have a few different models,
 * from the simple full triangle to different triangles, including ones with holes or with
 * rounded or missing "tips".
 */

#pragma once

#include <math.h>
#include "Entity.h"
#include "DoorSet.h"

class DoorSet;

class Door : public Entity {
public:

	Door(void);
	Door(DoorSet *doorSet, int index, float targetOpenness);
	Door(const Door &copy);
	virtual ~Door(void);

	Door &operator=(const Door &other);

	virtual void update(unsigned millisElapsed);

	virtual void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

	void setOpenness(float openness) {this->openness = openness; }
	float getOpenness() { return openness; }
	void setTargetOpenness(float targetOpenness) { this->targetOpenness = targetOpenness; }
	float getTargetOpenness() { return targetOpenness; }
	void setActive(bool active) { this->active = active; }
	void setDoorSet(DoorSet *doorSet) { this->doorSet = doorSet; }
	bool isActive() { return active; }
	/* Changes the closing speed for all doors */
	static void setClosingSpeed(float speed) { Door::closingSpeed = speed; }
	static float getClosingSpeed() { return Door::closingSpeed; }

	// The speed the a door should move when closing
	static float closingSpeed;

protected:

	/* Positions the door just outside the doorset, to enable it to close gradually from there */
	void setInitialDoorPosition();

	/* The "openness" of the door, ie. the percentage of the door that is opened, 0.0f being fully closed to the player, and 1.0f being fully opened */
	float openness;
	/* While the openness stores the current state of the door, every door can be opening or closing. */
	float targetOpenness;
	/* This tells the door in which position of the doorset it is. This serves to position and rotate the door correctly */
	int doorIndex;
	/* Indicates if the door closing mechanism is active, meaning that the door will start to close */
	bool active;
	/* The doorset in which this door belongs */
	DoorSet *doorSet;

};