#include "PanelItem.h"

PanelItem::PanelItem(void) : InterfaceItem(ITEM_PANEL) {
	items = new vector<InterfaceItem*>();
}

PanelItem::PanelItem(Vector2 &position, float rotation, Vector2 &size) : InterfaceItem(ITEM_PANEL, position, rotation, size) {
	items = new vector<InterfaceItem*>();
}

PanelItem::~PanelItem(void) {
	items->clear();
	delete items;
}

void PanelItem::onMouseMoved(Vector2 &position, Vector2 &amount) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseMoved(position, amount);
	}
}

void PanelItem::onMouseClick(Uint8 button, Vector2 &position) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseClick(button, position);
	}
}

void PanelItem::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseDoubleClick(button, position);
	}
}

void PanelItem::onMouseButtonDown(Uint8 button, Vector2 &position) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseButtonDown(button, position);
	}
}

void PanelItem::onMouseButtonUp(Uint8 button, Vector2 &position) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseButtonUp(button, position);
	}
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void PanelItem::onMouseWheelScroll(int amount) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseWheelScroll(amount);
	}
}

void PanelItem::onKeyPress(SDL_Keysym key) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onKeyPress(key);
	}
}

void PanelItem::onKeyDown(SDL_Keysym key) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onKeyDown(key);
	}
}

void PanelItem::onKeyUp(SDL_Keysym key) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onKeyUp(key);
	}
}

void PanelItem::update(unsigned millisElapsed) {
	// We build the model matrix by applying the position, rotation and size, in this order
	GameApp *gameApp = GameApp::getInstance();
	int windowHeight = gameApp->getWindowHeight();
	Vector2 *renderSize = new Vector2(*size);
	if (size->x == SIZE_NO_RESIZE && texture != nullptr) {
		renderSize->x = texture->getTextureWidth();
	}
	if (size->y == SIZE_NO_RESIZE && texture != nullptr) {
		renderSize->y = texture->getTextureHeight();
	}
	float realPosX = position->x + (renderSize->x / 2.0f);
	float realPosY = windowHeight - position->y - (renderSize->y / 2.0f);
	modelMatrix = Matrix4::Translation(Vector3(realPosX, realPosY, 0.0f));
	modelMatrix = modelMatrix * Matrix4::Rotation(rotation, Vector3(0, 0, 1));
	modelMatrix = modelMatrix * Matrix4::Scale(Vector3(renderSize->x / 2, renderSize->y / 2, 1.0f));
	delete renderSize;

	// For each child, we have to manually calculate its model matrix
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		// We call the default update of the child item, because that can be different from the
		// default as well
		(*it)->update(millisElapsed);
		// We build the model matrix by applying the position, rotation and size, in this order
		Vector2 *childRenderSize = new Vector2((*it)->getSize());
		if ((*it)->getSize().x == SIZE_NO_RESIZE && (*it)->getTexture() != nullptr) {
			childRenderSize->x = (*it)->getTexture()->getTextureWidth();
		}
		if ((*it)->getSize().y == SIZE_NO_RESIZE && (*it)->getTexture() != nullptr) {
			childRenderSize->y = (*it)->getTexture()->getTextureHeight();
		}
		float childRealPosX = ((*it)->getPosition().x + position->x) + (childRenderSize->x / 2.0f);
		float childRealPosY = (windowHeight - ((*it)->getPosition().y + position->y)) - (childRenderSize->y / 2.0f);
		Matrix4 childModelMatrix;
		// We calculate the child's model matrix again, making sure rotation works
		childModelMatrix = Matrix4::Translation(Vector3(realPosX, realPosY, 0.0f));
		childModelMatrix = childModelMatrix * Matrix4::Rotation(rotation, Vector3(0, 0, 1));
		childModelMatrix = childModelMatrix * Matrix4::Translation(Vector3(-realPosX, -realPosY, 0.0f));
		childModelMatrix = childModelMatrix * Matrix4::Translation(Vector3(childRealPosX, childRealPosY, 0.0f));
		childModelMatrix = childModelMatrix * Matrix4::Rotation((*it)->getRotation(), Vector3(0, 0, 1));
		childModelMatrix = childModelMatrix * Matrix4::Scale(Vector3(childRenderSize->x / 2, childRenderSize->y / 2, 1.0f));
		(*it)->setModelMatrix(childModelMatrix);
		delete childRenderSize;
	}
}

void PanelItem::draw(unsigned millisElapsed) {
	if (texture) {
		GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &modelMatrix);
		texture->bindTexture(program, TEXTURE0);
		Model::getQuad()->draw();
	}
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->draw(millisElapsed);
	}
}

void PanelItem::addItem(InterfaceItem *item) {
	if (item) {
		// If we don't have space to store the entity, make some!
		// I set this if to >= to always have an empty space in the array, just in case
		if ((items->size() + 1) >= items->capacity()) {
			items->reserve(items->capacity() + 10);
		}
		items->emplace_back(item);
	}
}

void PanelItem::removeItem(InterfaceItem *item) {
	items->erase(std::remove(items->begin(), items->end(), item), items->end());
	// If vector becomes too big, shrink it
	if (items->size() + 20 < items->capacity()) {
		items->shrink_to_fit();
	}
}