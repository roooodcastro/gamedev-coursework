#include "Level.h"

Level::Level() {
	entities = new std::vector<Entity*>();
	projectionMatrix = new Matrix4();
	cameraMatrix = new Matrix4();
}

Level::Level(LevelType type) {
	entities = new std::vector<Entity*>();
	projectionMatrix = new Matrix4();
	cameraMatrix = new Matrix4();
	levelType = type;
}

Level::Level(LevelType type, UserInterface *userInterface) {
	entities = new std::vector<Entity*>();
	projectionMatrix = new Matrix4();
	cameraMatrix = new Matrix4();
	levelType = type;
	this->userInterface = userInterface;
}

Level::~Level(void) {
	if (userInterface) {
		delete userInterface;
	}
	entities->clear();
	delete entities;
}

void Level::onMouseMoved(Vector2 &position, Vector2 &amount) {
	if (userInterface) {
		userInterface->onMouseMoved(position, amount);
	}
}

void Level::onMouseClick(Uint8 button, Vector2 &position) {
	if (userInterface) {
		userInterface->onMouseClick(button, position);
	}
}

void Level::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	if (userInterface) {
		userInterface->onMouseDoubleClick(button, position);
	}
}

void Level::onMouseButtonDown(Uint8 button, Vector2 &position) {
	if (userInterface) {
		userInterface->onMouseButtonDown(button, position);
	}
}

void Level::onMouseButtonUp(Uint8 button, Vector2 &position) {
	if (userInterface) {
		userInterface->onMouseButtonUp(button, position);
	}
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void Level::onMouseWheelScroll(int amount) {
	if (userInterface) {
		userInterface->onMouseWheelScroll(amount);
	}
}

void Level::onKeyPress(SDL_Keysym key) {
	if (userInterface) {
		userInterface->onKeyPress(key);
	}
}

void Level::onKeyDown(SDL_Keysym key) {
	Keyboard::getInstance()->keyDown(key);
	// Now we propagate this event to the interface items
	if (userInterface) {
		userInterface->onKeyDown(key);
	}
}

void Level::onKeyUp(SDL_Keysym key) {
	Keyboard::getInstance()->keyUp(key);
	// Now we propagate this event to the interface items
	if (userInterface) {
		userInterface->onKeyUp(key);
	}
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
	userInterface->update(millisElapsed);
	for (std::vector<Entity*>::iterator it = entities->begin(); it != entities->end(); ++it) {
		(*it)->update(millisElapsed);
	}
}

void Level::drawLevel(unsigned int millisElapsed) {
	// Should first draw the interface using a ortographic projection,
	// then switch to a perspective projection and draw all entities

	// Draw Interface
	GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, false, (float*) cameraMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, false, (float*) projectionMatrix);
	userInterface->draw(millisElapsed);

	// Draw Entities
	for (std::vector<Entity*>::iterator it = entities->begin(); it != entities->end(); ++it) {
		// We first get the right shader to use with this entity
		Shader shader = *(GameApp::getInstance()->getDefaultShader());
		if ((*it)->getCustomShader()) {
			shader = *((*it)->getCustomShader());
		}
		program = shader.getShaderProgram();
		glUseProgram(program);
		// Then we update the shader matrices
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &((*it)->getModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, false, (float*) cameraMatrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, false, (float*) projectionMatrix);
		//glUniformMatrix4fv(glGetUniformLocation(program, "textureMatrix"), 1, false, (float*)&textureMatrix);
		(*it)->draw(millisElapsed);
	}
}