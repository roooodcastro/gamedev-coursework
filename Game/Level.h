#pragma once

#include <vector>
#include <algorithm>
#include "UserInterface.h"
#include "Entity.h"
#include "GameApp.h"
#include "Matrix4.h"

/*
 * A enumerator with the possible level types. Each type will behave differently when it's being processed by the GameApp class.
 */
enum LevelType {
	LEVEL_MENU, // A interactive menu screen level, this can be a menu or a end game screen, etc
	LEVEL_STATIC_SCREEN, // A static screen level with no user input. Usually used as a loading screen
	LEVEL_GAME, // A game level, to be used when the player is actually playing a level
};

class UserInterface;
class Entity;

class Level {
public:

	Level(LevelType type);
	Level(LevelType type, UserInterface &userInterface);
	~Level(void);

	/* ==========================================
	 * ================= Events =================
	 * ==========================================
	 */

	/* Level related events */
	void load(); // Will fire to load the level and all its entities and interfaces
	void onStart(); // Will fire when the current level of the game is switched to this
	void onPause(); // Will fire when the gmae pauses
	void onResume(); // Will fire when the game restarts from a pause state
	void onFinish(); // Will fire when the current level of the game is switched to another one
	/* Mouse events */
	void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
	void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
	void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
	void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
	void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
	void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls
	/* Keyboard events */
	void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
	void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

	/* ==========================================
	 * ============ Game loop control ===========
	 * ==========================================
	 */

	void processLevelTick(unsigned int millisElapsed);
	void drawLevel(unsigned int millisElapsed);

	/* ==========================================
	 * =============== Other stuff ==============
	 * ==========================================
	 */

	// Adds a new entity to this level
	void addEntity(Entity *entity);
	// Removes a entity from this level
	bool removeEntity(Entity *entity);

	// General getters and setters
	void setCameraMatrix(Matrix4 &cameraMatrix) { (*this->cameraMatrix) = cameraMatrix; }
	Matrix4 getCameraMatrix() { return *cameraMatrix; }
	void setProjectionMatrix(Matrix4 &projectionMatrix) { (*this->projectionMatrix) = projectionMatrix; }
	Matrix4 getProjectionMatrix() { return *projectionMatrix; }
	void setInterface(UserInterface *userInterface) { this->userInterface = userInterface; }
	UserInterface *getUserInterface() { return userInterface; }

	Entity *getEntity(int index) {
		try {
			return entities->at(index);
		} catch (int &) {
			return NULL;
		}
	}
	std::vector<Entity*> *getEntities() { return entities; }

private:

	Level(void);

	std::vector<Entity*> *entities; // A list with all the entities contained in this level
	LevelType levelType; // The type of this level. MUST be set.

	UserInterface *userInterface; // The player interface, may be the manu, or a HUD
	Matrix4 *cameraMatrix; // The viewMatrix, or the camera
	Matrix4 *projectionMatrix; // The projectionMatrix. This will be switched all the time to render interface and game
};