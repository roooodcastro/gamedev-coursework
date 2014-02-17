#include "Level.h"

Level::Level() {
}

Level::Level(LevelType type) {
	levelType = type;
}

Level::Level(LevelType type, UserInterface *userInterface) {
	levelType = type;
	this->userInterface = userInterface;
}

Level::~Level(void) {
	if (userInterface) {
		delete userInterface;
	}
}

void Level::addEntity(Entity *entity) {
	if (entity) {
		// If we don't have space to store the entity, make some!
		// I set this if to >= to always have an empty space in the array, just in case
		if ((entities.size() + 1) >= entities.capacity()) {
			entities.reserve(entities.capacity() + 10);
		}
		entities.emplace_back(entity);
	}
}

bool Level::removeEntity(Entity *entity) {
	try {
		for (auto it = entities.begin(); it != entities.end();) {
			if((*it) == entity) {
				// Found the entity. Remove it from vector and return true
				entities.erase(it);
				return true;
			} else { ++it; }
		}
		// Didn't find the entity in the vector
		return false;
	} catch (int &e) {
		// An error occurred while trying toremove the entity
		return false;
	}
}

void Level::processLevelTick(unsigned int millisElapsed) {

}

void Level::drawLevel(unsigned int millisElapsed) {
	// Should first draw the interface using a ortographic projection,
	// then switch to a perspective projection and draw all entities

	// Draw Interface

	// Draw Entities
	
}