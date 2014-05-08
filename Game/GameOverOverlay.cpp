#include "GameOverOverlay.h"

GameOverOverlay::GameOverOverlay(void) {
	GameApp *app = GameApp::getInstance();
	centerText = new TextItem(Vector2(POSITION_CENTERED, 60), 0, "Game Over", 100);
	centerText->setFont("resources/fonts/Honeycombed.ttf");
	addInnerItem(centerText);
	this->texture = Texture::getOrCreate("0xAA000000", Colour(0xCC000000));
	this->position = new Vector2(0, 0);
	this->size = new Vector2((float) app->getWindowWidth(), (float) app->getWindowHeight());
	this->hidden = true;
	// Create the menu buttons
	restartButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 450), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	addInnerItem(restartButton);
	leaderboardButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 590), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	addInnerItem(leaderboardButton);
	menuButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 730), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	addInnerItem(menuButton);
	quitButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 870), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	addInnerItem(quitButton);

	// Draw some text over the buttons background
	TextItem *restartText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 470), 0, "Play Again", 72);
	restartText->setFont("resources/fonts/Neuropol.ttf");
	addInnerItem(restartText);
	TextItem *leaderboardText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 610), 0, "Leaderboard", 72);
	leaderboardText->setFont("resources/fonts/Neuropol.ttf");
	addInnerItem(leaderboardText);
	TextItem *menuText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 750), 0, "Quit to Menu", 72);
	menuText->setFont("resources/fonts/Neuropol.ttf");
	addInnerItem(menuText);
	TextItem *quitText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 890), 0, "Quit to Desktop", 72);
	quitText->setFont("resources/fonts/Neuropol.ttf");
	addInnerItem(quitText);

	// Draw the score of the player
	scoreText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 225), 0, "Total Distance: 00000m", 80);
	scoreText->setFont("resources/fonts/Neuropol.ttf");
	addInnerItem(scoreText);

}

GameOverOverlay::~GameOverOverlay(void) {
	delete centerText;
	delete leaderboardButton;
	delete restartButton;
	delete menuButton;
	delete quitButton;
}

void GameOverOverlay::onKeyPress(SDL_Keysym key) {

}

void GameOverOverlay::update(unsigned millisElapsed) {
	if (restartButton->isPressed()) {
		GameMenu *menu = new GameMenu();
		menu->setActionAfterFade(1);
		GameApp::getInstance()->setCurrentLevel(menu);
	}
	if (leaderboardButton->isPressed()) {
		LeaderboardScreen *leaderboard = new LeaderboardScreen();
		GameApp::getInstance()->setCurrentLevel(leaderboard);
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

void GameOverOverlay::setDistance(int distance) {
	this->distance = distance;
	char buffer[80];
	sprintf(buffer, "Total Distance: %05dm", distance);
	scoreText->setText(buffer);
}

void GameOverOverlay::setLeaderboardPos(int pos) {
	if (pos > 0) {
		std::string text = "";
		switch (pos) {
		case 1:
			text = "Congratulations! You've got FIRST place!";
			break;
		case 2:
			text = "Congratulations! You've got 2nd place!";
			break;
		case 3:
			text = "Congratulations! You've got 3rd place!";
			break;
		case 4:
			text = "Congratulations! You've got 4th place!";
			break;
		case 5:
			text = "Congratulations! You've got 5th place!";
			break;
		}
		TextItem *leaderboardText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 325), 0, text, 80);
		leaderboardText->setFont("resources/fonts/Neuropol.ttf");
		leaderboardText->setColour(Colour(0xFFFFDD00));
		addInnerItem(leaderboardText);
	}
}