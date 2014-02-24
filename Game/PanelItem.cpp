#include "PanelItem.h"

PanelItem::PanelItem(void) : InterfaceItem(ITEM_PANEL) {

}

PanelItem::PanelItem(Vector2 &position, float rotation, Vector2 &size) : InterfaceItem(ITEM_PANEL, position, rotation, size) {

}

PanelItem::~PanelItem(void) {

}

// TODO: botar pra dar update e draw nos childs, descobrir como cria textura de 1 pixel no código, implementar métodos de add e remove, implementar texto

void PanelItem::update(unsigned millisElapsed) {
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

void PanelItem::draw(unsigned millisElapsed) {
	if (texture) {
		GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &modelMatrix);
		texture->bindTexture(program, TEXTURE0);

		Model::getQuad()->draw();
	}
}