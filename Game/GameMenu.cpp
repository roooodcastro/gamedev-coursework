#include "GameMenu.h"

GameMenu::GameMenu(void) : Level(LEVEL_MENU, UserInterface()) {
	int windowWidth = GameApp::getInstance()->getWindowWidth();
	int windowHeight = GameApp::getInstance()->getWindowHeight();
	setProjectionMatrix(Matrix4::Perspective(1.0f, -100.0f, 1280.0f / 720.0f, 45.0f));
	setCameraMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	Texture *bgTexture = Texture::getOrCreate("MenuBg", "resources/images/HeXcape Menu Bg.png");
	ImageItem *background = new ImageItem(Vector2(), 0, Vector2((float) (windowWidth), (float) (windowHeight)), bgTexture);
	userInterface->addItem(background, "Background");
	// Create the menu buttons
	ButtonItem *gameButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 430), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	userInterface->addItem(gameButton, "GameButton");
	ButtonItem *leaderButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 580), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	userInterface->addItem(leaderButton, "LeaderButton");
	ButtonItem *helpButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 730), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	userInterface->addItem(helpButton, "HelpButton");
	ButtonItem *quitButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 880), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	userInterface->addItem(quitButton, "QuitButton");
	// Draw some text over the buttons background
	TextItem *gameText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 450), 0, "Start Game", 72);
	gameText->setFont("resources/fonts/Neuropol.ttf");
	userInterface->addItem(gameText, "GameText");
	TextItem *helpText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 750), 0, "Help & Credits", 72);
	helpText->setFont("resources/fonts/Neuropol.ttf");
	userInterface->addItem(helpText, "HelpText");
	TextItem *quitText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 900), 0, "Quit Game", 72);
	quitText->setFont("resources/fonts/Neuropol.ttf");
	userInterface->addItem(quitText, "QuitText");
	TextItem *leaderText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 600), 0, "Leaderboard", 72);
	leaderText->setFont("resources/fonts/Neuropol.ttf");
	userInterface->addItem(leaderText, "LeaderText");
	actionAfterFade = 0;
	// Set the ambience music
	Music *music = Music::getOrCreate("resources/audio/Doors.mp3", "MenuMusic");

	// Add buttons to change sound setting
	bool sound = ConfigurationManager::getInstance()->readBool("sound", true);
	ButtonItem *soundButton = new ButtonItem(Vector2((float) (windowWidth - 250), 880), 0, Vector2(120, 120), "SoundButton", "resources/images/SoundButton.png", "resources/images/SoundButtonHover.png", "resources/images/SoundButtonPressed.png", "resources/images/SoundButtonPressed.png");
	userInterface->addItem(soundButton, "SoundButton");
	soundButton->setHidden(!sound);
	ButtonItem *noSoundButton = new ButtonItem(Vector2((float) (windowWidth - 250), 880), 0, Vector2(120, 120), "NoSoundButton", "resources/images/NoSoundButton.png", "resources/images/NoSoundButtonHover.png", "resources/images/NoSoundButtonPressed.png", "resources/images/NoSoundButtonPressed.png");
	noSoundButton->setHidden(sound);
	userInterface->addItem(noSoundButton, "NoSoundButton");
}

GameMenu::~GameMenu(void) {
}

void GameMenu::onStart() {
	Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
	music->play(1000);
	userInterface->startFadeIn(200);
}

void GameMenu::onPause() {
}

void GameMenu::onResume() {
}

void GameMenu::onFinish() {
	Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
	Sound::stopAllSfx();
	music->stop(0);
}

void GameMenu::processLevelTick(unsigned int millisElapsed) {
	Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
	ButtonItem *gameButton = (ButtonItem*) userInterface->getItem("GameButton");
	ButtonItem *leaderButton = (ButtonItem*) userInterface->getItem("LeaderButton");
	ButtonItem *helpButton = (ButtonItem*) userInterface->getItem("HelpButton");
	ButtonItem *quitButton = (ButtonItem*) userInterface->getItem("QuitButton");
	ButtonItem *soundButton = (ButtonItem*) userInterface->getItem("SoundButton");
	ButtonItem *noSoundButton = (ButtonItem*) userInterface->getItem("NoSoundButton");
	if (gameButton->isPressed()) {
		music->stop(500);
		userInterface->startFadeOut(500);
		actionAfterFade = 1;
	}
	if (leaderButton->isPressed()) {
		music->stop(500);
		userInterface->startFadeOut(300);
		actionAfterFade = 2;
	}
	if (helpButton->isPressed()) {
		music->stop(500);
		userInterface->startFadeOut(300);
		actionAfterFade = 3;
	}
	if (quitButton->isPressed()) {
		music->stop(500);
		userInterface->startFadeOut(200);
		actionAfterFade = 4;
	}
	if (actionAfterFade > 0 && !userInterface->isFading()) {
		switch (actionAfterFade) {
		case 1: {
			RaceLevel *raceLevel = new RaceLevel();
			GameApp::getInstance()->setCurrentLevel(raceLevel);
			Sound::playSfx("SuccessSfx", 128);
			break;
		}
		case 2: {
			LeaderboardScreen *leaderboard = new LeaderboardScreen();
			GameApp::getInstance()->setCurrentLevel(leaderboard);
			break;
		}
		case 3: {
			HelpScreen *help = new HelpScreen();
			GameApp::getInstance()->setCurrentLevel(help);
			break;
		}
		case 4: {
			GameApp::getInstance()->exitGame();
			return;
		}
		}
	}

	if (soundButton->isPressed()) {
		soundButton->setHidden(true);
		soundButton->setPressed(false);
		noSoundButton->setHidden(false);
		Sound::setMute(true);
		Music::setMute(true);
		ConfigurationManager::getInstance()->writeConfig("sound", false);
	}
	if (noSoundButton->isPressed()) {
		soundButton->setHidden(false);
		noSoundButton->setHidden(true);
		noSoundButton->setPressed(false);
		Sound::setMute(false);
		Music::setMute(false);
		((Music*) ResourcesManager::getResource("MenuMusic"))->play(1000);
		ConfigurationManager::getInstance()->writeConfig("sound", true);
	}

	Level::processLevelTick(millisElapsed);
}