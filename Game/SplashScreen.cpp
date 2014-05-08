#include "SplashScreen.h"

SplashScreen::SplashScreen(void) : Level(LEVEL_STATIC_SCREEN, UserInterface()) {
	int windowWidth = GameApp::getInstance()->getWindowWidth();
	int windowHeight = GameApp::getInstance()->getWindowHeight();
	setProjectionMatrix(Matrix4::Perspective(1.0f, -100.0f, 1280.0f / 720.0f, 45.0f));
	setCameraMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	Texture *bgTexture = Texture::getOrCreate("SplashBg", "resources/images/SplashBg.png");
	ImageItem *background = new ImageItem(Vector2(), 0, Vector2((float) (windowWidth + 0), (float) (windowHeight + 0)), bgTexture);
	userInterface->addItem(background, "Background");
	TextItem *loadingText = new TextItem(Vector2(100, windowHeight - 250), 0, "LOADING", 72);
	loadingText->setFont("resources/fonts/Honeycombed.ttf");
	userInterface->addItem(loadingText, "LoadingText");
}

SplashScreen::~SplashScreen(void) {
}

void SplashScreen::onStart() {
	timeRunning = 0;
	lastAnim = 0;
}

void SplashScreen::onPause() {
}

void SplashScreen::onResume() {	
}

void  SplashScreen::onFinish() {
}

void SplashScreen::processLevelTick(unsigned int millisElapsed) {
	if (timeRunning > 100 && timeRunning < 2000) {
		// Load some stuff
		// Doesn't matter if we call this multiple times, as from the second time
		// it will only "get" the resource, and we don't care about FPS here anyway
		Model::getOrCreate("TRACK_DOOR_MESH", "resources/models/Door.mdl");
		Model::getOrCreate("TRACK_PIECE_1", "resources/models/TrackPiece.mdl");
		Model::getOrCreate("SHIP_MESH", "resources/models/testShip.mdl");
		Texture::getOrCreate("0xAA000000", Colour(0xAA000000));
		Sound::getOrCreate("resources/audio/sfx/success.wav", "SuccessSfx");
		Sound::getOrCreate("resources/audio/sfx/button_hover.wav", "ButtonHoverSfx");
		Sound::getOrCreate("resources/audio/sfx/button_click.wav", "ButtonClickSfx");
		Sound::getOrCreate("resources/audio/sfx/gameover.wav", "GameOverSfx");
		Sound::getOrCreate("resources/audio/sfx/explosion.wav", "ExplosionSfx");
		Sound::getOrCreate("resources/audio/sfx/speed_increased.wav", "SpeedIncreasedSfx");
		Sound::getOrCreate("resources/audio/sfx/door_passed.wav", "DoorPassedSfx");
		Sound::getOrCreate("resources/audio/sfx/racestart.wav", "RaceStartSfx");
		Music::getOrCreate("resources/audio/Doors.mp3", "MenuMusic");
		Texture::getOrCreate("MenuBg", "resources/images/HeXcape Menu Bg.png");
		Texture::getOrCreate("InstructionsBg", "resources/images/HeXcape Instructions.png");
		Texture::getOrCreate("GenericBg", "resources/images/HeXcapeGenericBg.png");

		// Load some configurations
		if (!ConfigurationManager::getInstance()->readBool("sound", true)) {
			Music::setMute(true);
			Sound::setMute(true);
		}
	}
	timeRunning += millisElapsed;
	if (timeRunning > (unsigned) (lastAnim + 100)) {
		lastAnim += 100;
		TextItem *loadingText = (TextItem*) userInterface->getItem("LoadingText");
		int numDots = ((lastAnim % 1000) / 100);
		std::string text = "Loading";
		for (int i = 0; i < numDots; i++) {
			text.append(".");
		}
		loadingText->setText(text);
	}
	if (timeRunning > 3850 && !userInterface->isFading()) {
		userInterface->startFadeOut(200);
	}
	if (timeRunning > 4000) {
		GameMenu *menu = new GameMenu();
		GameApp::getInstance()->setCurrentLevel(menu);
		return;
	}
	Level::processLevelTick(millisElapsed);
}