#include "Level.h"

Level::Level() {
	entities = new std::vector<Entity*>();
}

Level::Level(LevelType type) {
	entities = new std::vector<Entity*>();
	levelType = type;
}

Level::Level(LevelType type, UserInterface *userInterface) {
	entities = new std::vector<Entity*>();
	levelType = type;
	this->userInterface = userInterface;
}

Level::~Level(void) {
	if (userInterface) {
		delete userInterface;
	}
	delete entities;
}

void Level::addEntity(Entity *entity) {
	if (entity) {
		// If we don't have space to store the entity, make some!
		// I set this if to >= to always have an empty space in the array, just in case
		if ((entities->size() + 1) >= entities->capacity()) {
			entities->reserve(entities->capacity() + 10);
		}
		entities->emplace_back(entity);
	}
}

bool Level::removeEntity(Entity *entity) {
	try {
		for (auto it = entities->begin(); it != entities->end();) {
			if((*it) == entity) {
				// Found the entity. Remove it from vector and return true
				entities->erase(it);
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
	for(std::vector<Entity*>::iterator it = entities->begin(); it != entities->end(); ++it) {
		(*it)->update(millisElapsed);
	}
}

void Level::drawLevel(unsigned int millisElapsed) {
	// Should first draw the interface using a ortographic projection,
	// then switch to a perspective projection and draw all entities

	// Draw Interface
	//userInterface->draw(millisElapsed);

	// Draw Entities
	for(std::vector<Entity*>::iterator it = entities->begin(); it != entities->end(); ++it) {
		// We first get the right shader to use with this entity
		Shader shader = *(GameApp::getInstance()->getDefaultShader());
		if ((*it)->getCustomShader()) {
			shader = *((*it)->getCustomShader());
		}
		GLuint program = shader.getShaderProgram();
		glUseProgram(program);
		// Then we update the shader matrices
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix")  , 1, false, (float*) &((*it)->getModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix")   , 1, false, (float*) cameraMatrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix")   , 1, false, (float*) projectionMatrix);
		//glUniformMatrix4fv(glGetUniformLocation(program, "textureMatrix"), 1, false, (float*)&textureMatrix);
		(*it)->draw(millisElapsed);
	}
}