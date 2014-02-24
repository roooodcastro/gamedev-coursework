#include "InterfaceItem.h"

const float InterfaceItem::SIZE_NO_RESIZE = -1;

InterfaceItem::InterfaceItem(void) {
	position = new Vector2(0, 0);
	rotation = 0;
	size = new Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE);
	texture = NULL;
}

InterfaceItem::InterfaceItem(InterfaceItemType type) {
	position = new Vector2(0, 0);
	rotation = 0;
	size = new Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE);
	this->type = type;
	texture = NULL;
}

InterfaceItem::InterfaceItem(InterfaceItemType type, Vector2 &position, float rotation, Vector2 &size) {
	this->position = new Vector2(0, 0);
	this->rotation = 0;
	this->size = new Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE);
	if (&position != nullptr) {
		this->position = &position;
	}
	this->rotation = rotation;
	if (&size != nullptr) {
		this->size = &size;
	}
	this->type = type;
	texture = NULL;
}

InterfaceItem::~InterfaceItem(void) {
	delete position;
	delete size;
	delete texture;
}

bool InterfaceItem::isMouseHovering(Vector2 &mousePos) {
	if (mousePos.x >= position->x && mousePos.x <= (position->x + size->x)) {
		return mousePos.y >= position->y && mousePos.y <= (position->y + size->y);
	}
	return false;
}

void InterfaceItem::update(unsigned millisElapsed) {
	
	// Test code
	/*Level *level = GameApp::getInstance()->getCurrentLevel();
	if (level->isKeyPressed(SDLK_UP)) {
		this->position->y -= 1;
	}
	if (level->isKeyPressed(SDLK_DOWN)) {
		this->position->y += 1;
	}
	if (level->isKeyPressed(SDLK_LEFT)) {
		this->position->x -= 1;
	}
	if (level->isKeyPressed(SDLK_RIGHT)) {
		this->position->x += 1;
	}*/


	// We build the model matrix by applying the position, rotation and size, in this order
	GameApp *gameApp = GameApp::getInstance();
	int windowWidth = gameApp->getWindowWidth();
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
}

void InterfaceItem::draw(unsigned millisElapsed) {
	if (texture) {
		GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &modelMatrix);
		texture->bindTexture(program, TEXTURE0);

		Model::getQuad()->draw();
	}
}