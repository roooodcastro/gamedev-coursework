#include "Entity.h"

Entity::Entity(void) {
	this->childEntities = new std::vector<Entity*>();
	physicalBody = new PhysicalBody();
	modelMatrix = new Matrix4();
	customShader = NULL;
	model = NULL;
	parent = NULL;
}

Entity::Entity(const Entity &copy) {
	this->modelMatrix = new Matrix4(*(copy.modelMatrix));
	this->customShader = new Shader(*(copy.customShader));
	this->parent = new Entity(*(copy.parent));
	this->childEntities = new std::vector<Entity*>(*(copy.childEntities));
	this->model = new Model(*(copy.model));
	this->customShader = new Shader(*(copy.customShader));
	this->physicalBody = new PhysicalBody(*(copy.physicalBody));
}

Entity::Entity(Vector3 &position, Vector3 &velocity, Vector3 &rotation, Vector3 &scale) {
	this->childEntities = new std::vector<Entity*>();
	physicalBody = new PhysicalBody(0, 0, position);
	physicalBody->setRotation(rotation);
	physicalBody->setScale(scale);
	modelMatrix = new Matrix4();
	model = NULL;
	customShader = NULL;
	parent = NULL;
	
}

Entity::~Entity(void) {
	delete modelMatrix;
	delete childEntities;
	delete model;
	delete customShader;
	delete physicalBody;
}

Entity &Entity::operator=(const Entity &other) {
	*(this->modelMatrix) = *(other.modelMatrix);
	*(this->model) = *(other.model);
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
	*(this->physicalBody) = *(other.physicalBody);
	return *this;
}

void Entity::onMouseMoved(Vector2 &position, Vector2 &amount) {
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->onMouseMoved(position, amount);
	}
}

void Entity::onMouseClick(Uint8 button, Vector2 &position) {
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->onMouseClick(button, position);
	}
}

void Entity::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->onMouseDoubleClick(button, position);
	}
}

void Entity::onMouseButtonDown(Uint8 button, Vector2 &position) {
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->onMouseButtonDown(button, position);
	}
}

void Entity::onMouseButtonUp(Uint8 button, Vector2 &position) {
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->onMouseButtonUp(button, position);
	}
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void Entity::onMouseWheelScroll(int amount) {
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->onMouseWheelScroll(amount);
	}
}

void Entity::onKeyPress(SDL_Keysym key) {
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->onKeyPress(key);
	}
}

void Entity::onKeyDown(SDL_Keysym key) {
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->onKeyDown(key);
	}
}

void Entity::onKeyUp(SDL_Keysym key) {
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->onKeyUp(key);
	}
}


void Entity::calculateModelMatrix() {
	//Vector3 *calcPos = new Vector3(*(this->position));
	//Vector3 *calcVel = new Vector3(*(this->velocity));
	//Vector3 *calcRot = new Vector3(*(this->rotation));
	//Vector3 *calcSiz = new Vector3(*(this->scale));
	///*
	// * If is a child, we add up the parent's attributes.
	// *
	// * TODO: This method won't work if we have an entity that is a child of a child.
	// * In this case, the "grandson" will have its attributes relative to the first child,
	// * but the first child's attributes will be considered global, instead of relative
	// * to the parent's.
	// * Example: e3 is child of e2. e2 is child of e1.
	// * e1 pos: 10, 0, 0
	// * e2 pos: -2, 0, 0
	// * e3 pos: 5, 0, 0
	// * e3 final position will erroneous be 3, 0, 0, because the function doesn't check
	// * if the parent has a parent, and so on.
	// * In the unlikely event that my game has such complex entity relationship,
	// * I may or may not try to fix this.
	// */
	//if (parent != NULL) {
	//	*calcPos = *calcPos + *(parent->position);
	//	*calcVel = *calcVel + *(parent->velocity);
	//	*calcRot = *calcRot + *(parent->rotation);
	//	*calcSiz = *calcSiz * *(parent->scale);
	//}

	//// Now that we calculated the final attributes, build the matrix
	//Matrix4 rotationMatrix = Matrix4::Rotation(calcRot->x, Vector3(1, 0, 0)) * Matrix4::Rotation(calcRot->y, Vector3(0, 1, 0)) * Matrix4::Rotation(calcRot->z, Vector3(0, 0, 1));
	//*modelMatrix = Matrix4::Translation(*calcPos) * rotationMatrix * Matrix4::Scale(*calcSiz);

	//delete calcPos;
	//delete calcVel;
	//delete calcRot;
	//delete calcSiz;

	//// Finally, do the same for the children
	//for(std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
	//	// Bad things will happen if two entities are parent and child of each other at the same time, please don't do that.
	//	(*it)->calculateModelMatrix();
	//}

	Matrix4 rotX = Matrix4::Rotation(this->physicalBody->getRotation()->x, Vector3(1, 0, 0));
	Matrix4 rotY = Matrix4::Rotation(this->physicalBody->getRotation()->y, Vector3(0, 1, 0));
	Matrix4 rotZ = Matrix4::Rotation(this->physicalBody->getRotation()->z, Vector3(0, 0, 1));
	Matrix4 rotationMatrix = rotX * rotY * rotZ;
	*modelMatrix = Matrix4::Translation(*(this->physicalBody->getPosition())) * rotationMatrix * Matrix4::Scale(*(this->physicalBody->getScale()));
}

void Entity::update(unsigned millisElapsed) {
	// Apply all logic necessary

	/*
	 * Then we update the model matrix, only if this entity is not a child.
	 * This is because this function also update all children's matrices.
	 */
	if (parent == nullptr) {
		calculateModelMatrix();
	}
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->update(millisElapsed);
	}
}

void Entity::draw(unsigned millisElapsed) {
	if (model != NULL) {
		GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) modelMatrix);
		model->draw();
	}
	for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
		(*it)->draw(millisElapsed);
	}
}

void Entity::addChild(Entity *child) {
	if (child) {
		// If we don't have space to store the entity, make some!
		// I set this if to >= to always have an empty space in the array, just in case
		if ((childEntities->size() + 1) >= childEntities->capacity()) {
			childEntities->reserve(childEntities->capacity() + 5);
		}
		childEntities->emplace_back(child);
		child->parent = this;
	}
}

void Entity::removeChild(Entity *child) {
	if (child) {
		childEntities->erase(std::remove(childEntities->begin(), childEntities->end(), child), childEntities->end());
		// If vector becomes too big, shrink it
		if (childEntities->size() + 10 < childEntities->capacity()) {
			childEntities->shrink_to_fit();
		}
		child->parent = NULL;
	}
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

void Entity::setPhysicalBody(PhysicalBody &body) {
	*(this->physicalBody) = body;
}