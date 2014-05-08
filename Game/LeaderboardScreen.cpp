#include "LeaderboardScreen.h"

LeaderboardScreen::LeaderboardScreen(void) : Level(LEVEL_MENU, UserInterface()) {
	int windowWidth = GameApp::getInstance()->getWindowWidth();
	int windowHeight = GameApp::getInstance()->getWindowHeight();
	setProjectionMatrix(Matrix4::Perspective(1.0f, -100.0f, 1280.0f / 720.0f, 45.0f));
	setCameraMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	Texture *bgTexture = Texture::getOrCreate("GenericBg", "resources/images/HeXcapeGenericBg.png");
	ImageItem *background = new ImageItem(Vector2(), 0, Vector2((float) (windowWidth), (float) (windowHeight)), bgTexture);
	userInterface->addItem(background, "Background");
	// Create the menu buttons
	ButtonItem *backButton = new ButtonItem(Vector2(InterfaceItem::POSITION_CENTERED, 900), 0, Vector2(800, 120), "MenuButton", "resources/images/MenuButton.png", "resources/images/MenuButtonHov.png", "resources/images/MenuButtonPress.png", "resources/images/MenuButtonPress.png");
	// Draw some text over the buttons background
	TextItem *backText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 920), 0, "Back To Menu", 72);
	backText->setFont("resources/fonts/Neuropol.ttf");
	actionAfterFade = 0;
	// Create the title
	TextItem *title = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 40), 0, "Leaderboard", 120);
	title->setFont("resources/fonts/Honeycombed.ttf");
	userInterface->addItem(title, "TitleText");
	// Set the ambience music
	Music *music = Music::getOrCreate("resources/audio/Doors.mp3", "MenuMusic");

	userInterface->addItem(backButton, "ZBackButton");
	userInterface->addItem(backText, "ZBackText");

	// Create the actual scoreboard
	Texture *placeholder = Texture::getOrCreate("LeaderboardPlaceholder", "resources/images/LeaderboardPlaceholder.png");
	ImageItem *firstImage = new ImageItem(Vector2(InterfaceItem::POSITION_CENTERED, 220), 0, Vector2(1200, 120), placeholder);
	userInterface->addItem(firstImage, "FirstImage");
	ImageItem *secondImage = new ImageItem(Vector2(InterfaceItem::POSITION_CENTERED, 350), 0, Vector2(1200, 120), placeholder);
	userInterface->addItem(secondImage, "SecondImage");
	ImageItem *thirdImage = new ImageItem(Vector2(InterfaceItem::POSITION_CENTERED, 480), 0, Vector2(1200, 120), placeholder);
	userInterface->addItem(thirdImage, "ThirdImage");
	ImageItem *fourthImage = new ImageItem(Vector2(InterfaceItem::POSITION_CENTERED, 610), 0, Vector2(1200, 120), placeholder);
	userInterface->addItem(fourthImage, "FourthImage");
	ImageItem *fifthImage = new ImageItem(Vector2(InterfaceItem::POSITION_CENTERED, 740), 0, Vector2(1200, 120), placeholder);
	userInterface->addItem(fifthImage, "FifthImage");
	std::string firstString = ((std::string) "1st place: ").append(std::to_string((long long) Leaderboard::getInstance()->getScore(1)));
	std::string secondString = ((std::string) "2nd place: ").append(std::to_string((long long) Leaderboard::getInstance()->getScore(2)));
	std::string thirdString = ((std::string) "3rd place: ").append(std::to_string((long long) Leaderboard::getInstance()->getScore(3)));
	std::string fourthString = ((std::string) "4th place: ").append(std::to_string((long long) Leaderboard::getInstance()->getScore(4)));
	std::string fifthString = ((std::string) "5th place: ").append(std::to_string((long long) Leaderboard::getInstance()->getScore(5)));
	TextItem *firstText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 240), 0, firstString, 72);
	TextItem *secondText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 370), 0, secondString, 72);
	TextItem *thirdText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 500), 0, thirdString, 72);
	TextItem *fourthText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 630), 0, fourthString, 72);
	TextItem *fifthText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, 760), 0, fifthString, 72);
	firstText->setFont("resources/fonts/Neuropol.ttf");
	secondText->setFont("resources/fonts/Neuropol.ttf");
	thirdText->setFont("resources/fonts/Neuropol.ttf");
	fourthText->setFont("resources/fonts/Neuropol.ttf");
	fifthText->setFont("resources/fonts/Neuropol.ttf");
	userInterface->addItem(firstText, "FirstText");
	userInterface->addItem(secondText, "SecondText");
	userInterface->addItem(thirdText, "ThirdText");
	userInterface->addItem(fourthText, "FourthText");
	userInterface->addItem(fifthText, "FifthText");
}

LeaderboardScreen::~LeaderboardScreen(void) {
}

void LeaderboardScreen::onStart() {
	Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
	music->play(1000);
	userInterface->startFadeIn(200);
}

void LeaderboardScreen::onPause() {
}

void LeaderboardScreen::onResume() {
}

void LeaderboardScreen::onFinish() {
	Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
	Sound::stopAllSfx();
	music->stop(0);
}

void LeaderboardScreen::processLevelTick(unsigned int millisElapsed) {
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