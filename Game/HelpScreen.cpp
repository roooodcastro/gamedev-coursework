#include "HelpScreen.h"

HelpScreen::HelpScreen(void) : Level(LEVEL_MENU, UserInterface()) {
	int windowWidth = GameApp::getInstance()->getWindowWidth();
	int windowHeight = GameApp::getInstance()->getWindowHeight();
	setProjectionMatrix(Matrix4::Perspective(1.0f, -100.0f, 1280.0f / 720.0f, 45.0f));
	setCameraMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	Texture *bgTexture = Texture::getOrCreate("InstructionsBg", "resources/images/HeXcape Instructions.png");
	ImageItem *background = new ImageItem(Vector2(), 0, Vector2((float) (windowWidth), (float) (windowHeight)), bgTexture);
	userInterface->addItem(background, "BackgroundHelp");
	// Create the menu buttons
	ButtonItem *backButton = new ButtonItem(Vector2(900, 840), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	// Draw some text over the buttons background
	TextItem *backText = new TextItem(Vector2(1020, 860), 0, "Back To Menu", 72);
	backText->setFont("resources/fonts/Neuropol.ttf");
	actionAfterFade = 0;
	// Set the ambience music
	Music *music = Music::getOrCreate("resources/audio/Doors.mp3", "MenuMusic");

	userInterface->addItem(backButton, "ZBackButton");
	userInterface->addItem(backText, "ZBackText");
}

HelpScreen::~HelpScreen(void) {
}

void HelpScreen::onStart() {
	Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
	music->play(1000);
	userInterface->startFadeIn(200);
}

void HelpScreen::onPause() {
}

void HelpScreen::onResume() {
}

void HelpScreen::onFinish() {
	Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
	Sound::stopAllSfx();
	music->stop(0);
}

void HelpScreen::processLevelTick(unsigned int millisElapsed) {
	Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
	ButtonItem *backButton = (ButtonItem*) userInterface->getItem("ZBackButton");
	if (backButton->isPressed()) {
		music->stop(500);
		userInterface->startFadeOut(200);
		actionAfterFade = 1;
	}
	if (actionAfterFade > 0 && !userInterface->isFading()) {
		GameMenu *menu = new GameMenu();
		GameApp::getInstance()->setCurrentLevel(menu);
	}

	Level::processLevelTick(millisElapsed);
}