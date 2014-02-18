#pragma once

#include <vector>
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
	Level(LevelType type, UserInterface *userInterface);
	~Level(void);

	void load();
	void onStart();
	void onFinish();

	void processLevelTick(unsigned int millisElapsed);
	void drawLevel(unsigned int millisElapsed);

	// Adds a new entity to this level
	void addEntity(Entity *entity);
	// Removes a entity from this level
	bool removeEntity(Entity *entity);

	void setCameraMatrix(Matrix4 &cameraMatrix) { this->cameraMatrix = &cameraMatrix; }
	Matrix4 getCameraMatrix() { return *cameraMatrix; }
	void setProjectionMatrix(Matrix4 &projectionMatrix) { this->projectionMatrix = &projectionMatrix; }
	Matrix4 getProjectionMatrix() { return *projectionMatrix; }

	Entity *getEntity(int index) {
		try {
			return entities->at(index); 
		} catch (int &e) {
			return NULL;
		}
	}
	std::vector<Entity*> *getEntities() { return entities; }

private:

	Level(void);

	/*
	 * A list with all the entities contained in this level
	 */
	std::vector<Entity*> *entities;

	/*
	 * The type of this level. MUST be set.
	 */
	LevelType levelType;

	UserInterface *userInterface; // The player interface, may be the manu, or a HUD
	Matrix4 *cameraMatrix; // The viewMatrix, or the camera
	Matrix4 *projectionMatrix; // The projectionMatrix. This will be switched all the time to render interface and game
};