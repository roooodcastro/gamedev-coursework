#include "PauseMenu.h"

PauseMenu::PauseMenu(void) : PanelItem() {
	GameApp *app = GameApp::getInstance();
	pauseText = new TextItem(Vector2(POSITION_CENTERED, 100), 0, "Game Paused", 72);
	pauseText->setFont("resources/fonts/Honeycombed.ttf");
	addInnerItem(pauseText);
	this->texture = Texture::getOrCreate("0xAA000000", Colour(0xAA000000));
	this->position = new Vector2(0, 0);
	this->size = new Vector2((float) app->getWindowWidth(), (float) app->getWindowHeight());
	this->hidden = true;
	// Create the menu buttons
	continueButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 350), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	addInnerItem(continueButton);
	restartButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 490), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	addInnerItem(restartButton);
	menuButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 630), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	addInnerItem(menuButton);
	quitButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 770), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	addInnerItem(quitButton);

	// Draw some text over the buttons background
	TextItem *continueText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 370), 0, "Continue", 72);
	continueText->setFont("resources/fonts/Neuropol.ttf");
	addInnerItem(continueText);
	TextItem *restartText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 510), 0, "Restart Game", 72);
	restartText->setFont("resources/fonts/Neuropol.ttf");
	addInnerItem(restartText);
	TextItem *menuText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 650), 0, "Quit to Menu", 72);
	menuText->setFont("resources/fonts/Neuropol.ttf");
	addInnerItem(menuText);
	TextItem *quitText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 790), 0, "Quit to Desktop", 72);
	quitText->setFont("resources/fonts/Neuropol.ttf");
	addInnerItem(quitText);
}

PauseMenu::PauseMenu(const PauseMenu &copy) : PanelItem(copy) {
}

PauseMenu::~PauseMenu(void) {
	delete pauseText;
	delete continueButton;
	delete restartButton;
	delete menuButton;
	delete quitButton;
}

void PauseMenu::onKeyPress(SDL_Keysym key) {
}

void PauseMenu::update(unsigned millisElapsed) {
	if (continueButton->isPressed()) {
		GameApp::getInstance()->setGamePaused(false);
		continueButton->setPressed(false);
	}
	if (restartButton->isPressed()) {
		RaceLevel *newLevel = new RaceLevel();
		GameApp::getInstance()->setCurrentLevel(newLevel);
	}
	if (menuButton->isPressed()) {
		GameMenu *menu = new GameMenu();
		GameApp::getInstance()->setCurrentLevel(menu);
	}
	if (quitButton->isPressed()) {
		GameApp::getInstance()->exitGame();
	}
	InterfaceItem::update(millisElapsed);
}