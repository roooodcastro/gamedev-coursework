#include "InterfaceItem.h"

const float InterfaceItem::SIZE_NO_RESIZE = -1;
const float InterfaceItem::POSITION_CENTERED = -123456;

InterfaceItem::InterfaceItem(void) {
	position = new Vector2(0, 0);
	rotation = 0;
	size = new Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE);
	innerItems = new std::vector<InterfaceItem*>();
	texture = NULL;
	hidden = false;
}

InterfaceItem::InterfaceItem(const InterfaceItem &copy) {
	this->position = new Vector2(*(copy.position));
	this->rotation = copy.rotation;
	this->size = new Vector2(*(copy.size));
	this->type = copy.type;
	this->texture = new Texture(*(copy.texture));
	this->innerItems = new std::vector<InterfaceItem*>(*(copy.innerItems));
	this->modelMatrix = Matrix4(copy.modelMatrix);
	this->hidden = copy.hidden;
}

InterfaceItem::InterfaceItem(InterfaceItemType type) {
	position = new Vector2(0, 0);
	rotation = 0;
	size = new Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE);
	this->type = type;
	texture = NULL;
	innerItems = new std::vector<InterfaceItem*>();
	hidden = false;
}

InterfaceItem::InterfaceItem(InterfaceItemType type, Vector2 &position, float rotation, Vector2 &size) {
	this->position = new Vector2(0, 0);
	this->rotation = 0;
	this->size = new Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE);
	if (&position != nullptr) {
		*(this->position) = position;
	}
	this->rotation = rotation;
	if (&size != nullptr) {
		*(this->size) = size;
	}
	this->type = type;
	this->innerItems = new std::vector<InterfaceItem*>();
	texture = NULL;
	hidden = false;
}

InterfaceItem::~InterfaceItem(void) {
	delete position;
	delete size;
	delete texture;
	innerItems->clear();
	delete innerItems;
}

bool InterfaceItem::isMouseHovering(Vector2 &mousePos) {
	Vector2 realSize = getRealSize();
	Vector2 realPos = Vector2(*position);
	GameApp *gameApp = GameApp::getInstance();
	if (realPos.x == POSITION_CENTERED) {
		realPos.x = (gameApp->getWindowWidth() / 2.0f) - (realSize.x / 2.0f);
	}
	if (realPos.y == POSITION_CENTERED) {
		realPos.y = (gameApp->getWindowHeight() / 2.0f) - (realSize.y / 2.0f);
	}
	if (mousePos.x >= realPos.x && mousePos.x <= (realPos.x + realSize.x)) {
		return mousePos.y >= realPos.y && mousePos.y <= (realPos.y + realSize.y);
	}
	return false;
}

void InterfaceItem::update(unsigned millisElapsed) {
	if (!hidden) {
		// We build the model matrix by applying the position, rotation and size, in this order
		GameApp *gameApp = GameApp::getInstance();
		int windowWidth = gameApp->getWindowWidth();
		int windowHeight = gameApp->getWindowHeight();
		Vector2 *renderSize = new Vector2(*size);
		Vector2 *renderPos = new Vector2(*position);
		if (size->x == SIZE_NO_RESIZE && texture != nullptr) {
			renderSize->x = (float) texture->getTextureWidth();
		}
		if (size->y == SIZE_NO_RESIZE && texture != nullptr) {
			renderSize->y = (float) texture->getTextureHeight();
		}
		if (position->x == POSITION_CENTERED) {
			renderPos->x = (gameApp->getWindowWidth() / 2.0f) - (renderSize->x / 2.0f);
		}
		if (position->y == POSITION_CENTERED) {
			renderPos->y = (gameApp->getWindowHeight() / 2.0f) - (renderSize->y / 2.0f);
		}
		float realPosX = renderPos->x + (renderSize->x / 2.0f);
		float realPosY = windowHeight - renderPos->y - (renderSize->y / 2.0f);
		modelMatrix = Matrix4::Translation(Vector3(realPosX, realPosY, 0.0f));
		modelMatrix = modelMatrix * Matrix4::Rotation(rotation, Vector3(0, 0, 1));
		modelMatrix = modelMatrix * Matrix4::Scale(Vector3(renderSize->x / 2, renderSize->y / 2, 1.0f));
		delete renderSize;

		// For each child, we have to manually calculate its model matrix
		for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
			// We call the default update of the child item, because that can be different from the
			// default as well
			(*it)->update(millisElapsed);
			// We build the model matrix by applying the position, rotation and size, in this order
			Vector2 *childRenderSize = new Vector2((*it)->getSize());
			Vector2 *childRenderPos = new Vector2((*it)->getPosition());
			if ((*it)->getSize().x == SIZE_NO_RESIZE && (*it)->getTexture() != nullptr) {
				childRenderSize->x = (float) (*it)->getTexture()->getTextureWidth();
			}
			if ((*it)->getSize().y == SIZE_NO_RESIZE && (*it)->getTexture() != nullptr) {
				childRenderSize->y = (float) (*it)->getTexture()->getTextureHeight();
			}
			if ((*it)->getPosition().x == POSITION_CENTERED) {
				childRenderPos->x = (gameApp->getWindowWidth() / 2.0f) - (childRenderSize->x / 2.0f);
			}
			if ((*it)->getPosition().y == POSITION_CENTERED) {
				childRenderPos->y = (gameApp->getWindowHeight() / 2.0f) - (childRenderSize->y / 2.0f);
			}
			float childRealPosX = (childRenderPos->x + renderPos->x) + (childRenderSize->x / 2.0f);
			float childRealPosY = (windowHeight - (childRenderPos->y + renderPos->y)) - (childRenderSize->y / 2.0f);
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
			delete childRenderPos;
		}
		delete renderPos;
	}
}

void InterfaceItem::draw(unsigned millisElapsed, GLuint program) {
	if (!hidden) {
		if (texture) {
			glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &modelMatrix);
			texture->bindTexture(program, TEXTURE0);
			Model::getQuadMesh()->draw();
		}	
		for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
			(*it)->draw(millisElapsed, program);
		}
	}
}

Vector2 InterfaceItem::getRealSize() {
	Vector2 realSize = Vector2(*size);
	if (size->x == SIZE_NO_RESIZE && texture != nullptr) {
		realSize.x = (float) texture->getTextureWidth();
	}
	if (size->y == SIZE_NO_RESIZE && texture != nullptr) {
		realSize.y = (float) texture->getTextureHeight();
	}
	return realSize;
}

InterfaceItem &InterfaceItem::operator=(const InterfaceItem &other) {
	this->position = other.position;
	this->rotation = other.rotation;
	this->size = other.size;
	this->type = other.type;
	this->texture = other.texture;
	this->modelMatrix = other.modelMatrix;
	return *this;
}

void InterfaceItem::addInnerItem(InterfaceItem *item) {
	if (item) {
		// If we don't have space to store the item, make some!
		// I set this if to >= to always have an empty space in the array, just in case
		if ((innerItems->size() + 1) >= innerItems->capacity()) {
			innerItems->reserve(innerItems->capacity() + 5);
		}
		innerItems->emplace_back(item);
	}
}

void InterfaceItem::removeInnerItem(InterfaceItem *item) {
	innerItems->erase(std::remove(innerItems->begin(), innerItems->end(), item), innerItems->end());
	// If vector becomes too big, shrink it
	if (innerItems->size() + 10 < innerItems->capacity()) {
		innerItems->shrink_to_fit();
	}
}