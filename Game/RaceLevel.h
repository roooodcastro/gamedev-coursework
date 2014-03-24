/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is the main race level. It contains the ship, the race track,
 * a pause menu and the HUD.
 */

#pragma once

#include "Level.h"
#include "PauseMenu.h"
#include "Track.h"
#include "Ship.h"

class RaceLevel : public Level {
public:

	RaceLevel(void);
	RaceLevel(const RaceLevel &copy);
	virtual ~RaceLevel(void);

	/* Level related events */
	virtual void onStart(); // Will fire when the current level of the game is switched to this
	virtual void onPause(); // Will fire when the gmae pauses
	virtual void onResume(); // Will fire when the game restarts from a pause state
	virtual void onFinish(); // Will fire when the current level of the game is switched to another one
	/* Mouse events */
	virtual void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
	virtual void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
	virtual void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
	virtual void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
	virtual void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
	virtual void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls
	/* Keyboard events */
	virtual void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	virtual void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
	virtual void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

	virtual void processLevelTick(unsigned int millisElapsed);

protected:

	virtual void calculateCameraMatrix();

	/* A pause menu */
	PauseMenu *pauseMenu;

	/* The race track, containing all track pieces and generating and deleting track pieces as necessary*/
	Track *track;

	/* Internal counters to control track pieces names */
	int lastPieceDeleted;
	int lastPieceAdded;

	/* The target speed for the ship */
	float targetSpeed;

};