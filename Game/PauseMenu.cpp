#include "PauseMenu.h"

PauseMenu::PauseMenu(void) : PanelItem() {
	GameApp *app = GameApp::getInstance();
	pauseText = new TextItem(Vector2(POSITION_CENTERED, 100), 0, "Game Paused", 72);
	addInnerItem(pauseText);
	this->texture = Texture::createColourTexture(0xAA000000);
	this->position = new Vector2(0, 0);
	this->size = new Vector2((float) app->getWindowWidth(), (float) app->getWindowHeight());
	this->hidden = true;
}

PauseMenu::PauseMenu(const PauseMenu &copy) : PanelItem(copy) {

}

PauseMenu::~PauseMenu(void) {
	delete pauseText;
}

void PauseMenu::onKeyPress(SDL_Keysym key) {
}