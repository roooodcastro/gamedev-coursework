#include "ButtonItem.h"

ButtonItem::ButtonItem(void) : InterfaceItem(ITEM_BUTTON) {
	state = BUTTON_NORMAL;
}

ButtonItem::ButtonItem(Vector2 &position, float rotation, Vector2 &size) : InterfaceItem(ITEM_BUTTON, position, rotation, size) {
	state = BUTTON_NORMAL;
}

ButtonItem::ButtonItem(Vector2 &position, float rotation, Vector2 &size, Texture *normal, Texture *hovered, Texture *pressed, Texture *selected) :
InterfaceItem(ITEM_BUTTON, position, rotation, size) {
	this->normalTex = normal;
	this->texture = this->normalTex;
	this->hoveredTex = hovered;
	this->pressedTex = pressed;
	this->selectedTex = selected;
}

ButtonItem::ButtonItem(Vector2 &position, float rotation, Vector2 &size, char *normal, char *hovered, char *pressed, char *selected) : InterfaceItem(ITEM_BUTTON, position, rotation, size) {
	this->normalTex = new Texture(normal);
	this->texture = this->normalTex;
	this->hoveredTex = new Texture(hovered);
	this->pressedTex = new Texture(pressed);
	this->selectedTex = new Texture(selected);
}

ButtonItem::~ButtonItem(void) {
	delete normalTex;
	delete hoveredTex;
	delete pressedTex;
	delete selectedTex;
}

void ButtonItem::onMouseMoved(Vector2 &position, Vector2 &amount) {
	if (state != BUTTON_PRESSED) {
		if (isMouseHovering(position)) {
			state = BUTTON_HOVERED;
		} else if (selected) {
			state = BUTTON_SELECTED;
		} else {
			state = BUTTON_NORMAL;
		}
	}
}

void ButtonItem::onMouseClick(Uint8 button, Vector2 &position) {
	std::cout << "Clicked!" << std::endl;
}

void ButtonItem::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	// UNUSED
}

void ButtonItem::onMouseButtonDown(Uint8 button, Vector2 &position) {
	if (isMouseHovering(position) && button == SDL_BUTTON_LEFT) {
		state = BUTTON_PRESSED;
	}
}

void ButtonItem::onMouseButtonUp(Uint8 button, Vector2 &position) {
	if (state == BUTTON_PRESSED && button == SDL_BUTTON_LEFT) {
		if (isMouseHovering(position)) {
			state = BUTTON_HOVERED;
			onMouseClick(button, position);
		} else if (selected) {
			state = BUTTON_SELECTED;
		} else {
			state = BUTTON_NORMAL;
		}
	}
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void ButtonItem::onMouseWheelScroll(int amount) {
}

void ButtonItem::onKeyPress(SDL_Keysym key) {
}

void ButtonItem::onKeyDown(SDL_Keysym key) {
}

void ButtonItem::onKeyUp(SDL_Keysym key) {
}

void ButtonItem::setTexture(Texture *texture) {
	this->texture = texture;
	this->normalTex = texture;
	this->hoveredTex = texture;
	this->pressedTex = texture;
	this->selectedTex = texture;
}

void ButtonItem::draw(unsigned millisElapsed) {
	Texture *chosenTex = normalTex;
	switch (state) {
	//case BUTTON_NORMAL:
		//break;
	case BUTTON_HOVERED:
		chosenTex = hoveredTex;
		break;
	case BUTTON_PRESSED:
		chosenTex = pressedTex;
		break;
	case BUTTON_SELECTED:
		chosenTex = selectedTex;
		break;
	}
	if (&chosenTex) {
		GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &modelMatrix);
		chosenTex->bindTexture(program, TEXTURE0);
		Model::getQuad()->draw();
	} else if (normalTex) {
		// Fallback in case one of the textures wasn't specified
		GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &modelMatrix);
		normalTex->bindTexture(program, TEXTURE0);
		Model::getQuad()->draw();
	}
}