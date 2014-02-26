#include "Entity.h"

Entity::Entity(void) {
	this->childEntities = new std::vector<Entity*>();
	position = new Vector3(0, 0, 0);
	velocity = new Vector3(0, 0, 0);
	rotation = new Vector3(0, 0, 0);
	scale = new Vector3(1.0f, 1.0f, 1.0f);
	modelMatrix = new Matrix4();
	customShader = NULL;
	parent = NULL;
	dragging = false;
	draggingRight = false;
}

Entity::Entity(const Entity &copy) {
	this->position = new Vector3(*(copy.position));
	this->velocity = new Vector3(*(copy.velocity));
	this->rotation = new Vector3(*(copy.rotation));
	this->scale = new Vector3(*(copy.scale));
	this->modelMatrix = new Matrix4(*(copy.modelMatrix));
	this->customShader = new Shader(*(copy.customShader));
	this->parent = new Entity(*(copy.parent));
	this->childEntities = new std::vector<Entity*>(*(copy.childEntities));
}

Entity::Entity(Vector3 &position, Vector3 &velocity, Vector3 &rotation, Vector3 &scale) {
	this->childEntities = new std::vector<Entity*>();
	this->position = new Vector3(position);
	this->velocity = new Vector3(velocity);
	this->rotation = new Vector3(rotation);
	this->scale = new Vector3(scale);
	modelMatrix = new Matrix4();
	customShader = NULL;
	parent = NULL;
	dragging = false;
	draggingRight = false;
}

Entity::~Entity(void) {
	delete position;
	delete velocity;
	delete rotation;
	delete scale;
	delete modelMatrix;
	delete childEntities;
}

Entity &Entity::operator=(const Entity &other) {
	*(this->position) = *(other.position);
	*(this->velocity) = *(other.velocity);
	*(this->rotation) = *(other.rotation);
	*(this->scale) = *(other.scale);
	*(this->modelMatrix) = *(other.modelMatrix);
	if (customShader != NULL) {
		if (other.customShader == NULL) {
			this->customShader = NULL;
		} else {
			*(this->customShader) = *(other.customShader);
		}
	} else {
		if (other.customShader != NULL) {
			this->customShader = new Shader(*(other.customShader));
		}
	}
	if (parent != NULL) {
		if (other.parent == NULL) {
			this->parent = NULL;
		} else {
			*(this->parent) = *(other.parent);
		}
	} else {
		if (other.parent != NULL) {
			this->parent = new Entity(*(other.parent));
		}
	}
	*(this->childEntities) = *(other.childEntities);
	return *this;
}

void Entity::onMouseMoved(Vector2 &position, Vector2 &amount) {
	if (dragging) {
		this->rotation->y += (amount.x / 4.0f);
		this->rotation->x += (amount.y / 4.0f);
	} else if (draggingRight) {
		this->position->x += (amount.x / 20.0f);
		this->position->y -= (amount.y / 20.0f);
	}
}

void Entity::onMouseClick(Uint8 button, Vector2 &position) {
}

void Entity::onMouseDoubleClick(Uint8 button, Vector2 &position) {
}

void Entity::onMouseButtonDown(Uint8 button, Vector2 &position) {
	if (button == SDL_BUTTON_MIDDLE) {
		dragging = true;
	} else if (button == SDL_BUTTON_RIGHT) {
		draggingRight = true;
	}
}

void Entity::onMouseButtonUp(Uint8 button, Vector2 &position) {
	if (button == SDL_BUTTON_MIDDLE) {
		dragging = false;
	} else if (button == SDL_BUTTON_RIGHT) {
		draggingRight = false;
	}
}

void Entity::onMouseWheelScroll(int amount) {
	this->position->z += amount * 2;
}

void Entity::onKeyPress(SDL_Keysym key) {
}

void Entity::onKeyDown(SDL_Keysym key) {
}

void Entity::onKeyUp(SDL_Keysym key) {
}

void Entity::calculateModelMatrix() {
	Vector3 *calcPos = this->position;
	Vector3 *calcVel = this->velocity;
	Vector3 *calcRot = this->rotation;
	Vector3 *calcSiz = this->scale;
	/*
	 * If is a child, we add up the parent's attributes.
	 *
	 * TODO: This method won't work if we have an entity that is a child of a child.
	 * In this case, the "grandson" will have its attributes relative to the first child,
	 * but the first child's attributes will be considered global, instead of relative
	 * to the parent's.
	 * Example: e3 is child of e2. e2 is child of e1.
	 * e1 pos: 10, 0, 0
	 * e2 pos: -2, 0, 0
	 * e3 pos: 5, 0, 0
	 * e3 final position will erroneous be 3, 0, 0, because the function doesn't check
	 * if the parent has a parent, and so on.
	 * In the unlikely event that my game has such complex entity relationship,
	 * I may or may not try to fix this.
	 */
	if (parent != NULL) {
		*calcPos = *calcPos + *(parent->position);
		*calcVel = *calcVel + *(parent->velocity);
		*calcRot = *calcRot + *(parent->rotation);
		*calcSiz = *calcSiz * *(parent->scale);
	}

	// Now that we calculated the final attributes, build the matrix
	Matrix4 rotationMatrix = Matrix4::Rotation(calcRot->x, Vector3(1, 0, 0)) * Matrix4::Rotation(calcRot->y, Vector3(0, 1, 0)) * Matrix4::Rotation(calcRot->z, Vector3(0, 0, 1));
	*modelMatrix = Matrix4::Translation(*calcPos) * rotationMatrix * Matrix4::Scale(*calcSiz);

	// Finally, do the same for the children
	for(std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		// Bad things will happen if two entities are parent and child of each other at the same time, please don't do that.
		(*it)->calculateModelMatrix();
	}
}

void Entity::update(unsigned millisElapsed) {
	// Apply all logic necessary

	// TODO: apply some logic

	/*
	 * Then we update the model matrix, only if this entity is not a child.
	 * This is because this function also update all children's matrices.
	 */
	if (parent == nullptr) {
		calculateModelMatrix();
	}
}

void Entity::draw(unsigned millisElapsed) {
	if (model) {
		GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
		Texture *white = Texture::createColourTexture(0xFFFFFFFF);
		white->bindTexture(program, TEXTURE0);
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) modelMatrix);
		model->draw();
		delete white;
	}
}

void Entity::addChild(Entity *child) {
	// TODO
}

void Entity::removeChild(Entity *child) {
	// TODO
}

void Entity::makeOrphan() {
	if (parent) {
		parent->removeChild(this);
	}
}

void Entity::setCustomShader(Shader &customShader) {
	if (this->customShader == NULL) {
		this->customShader = new Shader(customShader);
	} else {
		*(this->customShader) = customShader;
	}
}