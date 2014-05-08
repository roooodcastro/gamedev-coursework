#include "RaceLevel.h"

RaceLevel::RaceLevel(void) : Level(LEVEL_GAME, UserInterface()) {
	userInterface->addItem(new PauseMenu(), "PauseMenu");
	userInterface->addItem(new GameOverOverlay(), "GameOverOverlay");
	int windowWidth = GameApp::getInstance()->getWindowWidth();
	int windowHeight = GameApp::getInstance()->getWindowHeight();
	// Add the HUD to the interface
	TextItem *speedometer = new TextItem(Vector2(windowWidth - 420.0f, windowHeight - 100.0f), 0, "000 km/h", 64);
	speedometer->setColour(Colour(255, 255, 255, 255));
	speedometer->setFont("resources/fonts/Neuropol.ttf");
	userInterface->addItem(speedometer, "Speedometer");
	TextItem *distmeter = new TextItem(Vector2(50.0f, windowHeight - 100.0f), 0, "Distance: 0m", 64);
	distmeter->setColour(Colour(255, 255, 255, 255));
	distmeter->setFont("resources/fonts/Neuropol.ttf");
	userInterface->addItem(distmeter, "DistanceMeter");
	TextItem *speedIncreaseIndicator = new TextItem(Vector2(windowWidth - 490.0f, windowHeight - 160.0f), 0, "Speed Increased", 48);
	speedIncreaseIndicator->setFont("resources/fonts/Neuropol.ttf");
	speedIncreaseIndicator->setColour(Colour(0xFFFF4444));
	speedIncreaseIndicator->setHidden(true);
	userInterface->addItem(speedIncreaseIndicator, "SpeedIncIndicator");
	// Add the center screen text to the interface
	TextItem *centerText = new TextItem(Vector2(InterfaceItem::POSITION_CENTERED, InterfaceItem::POSITION_CENTERED), 0, "GET READY", 150);
	centerText->setFont("resources/fonts/Neuropol.ttf");
	userInterface->addItem(centerText, "CenterText");

	// Lights!
	Vector3 lightColour = Vector3(0.5f, 0.5f, 0.5f);
	addLightSource(*(new Light(Vector3(100, 0, 0), 25, lightColour)));
	addLightSource(*(new Light(Vector3(-100, 0, 0), 25, lightColour)));
	addLightSource(*(new Light(Vector3(0, 100, 0), 25, lightColour)));
	addLightSource(*(new Light(Vector3(0, -100, 0), 25, lightColour)));
	addLightSource(*(new Light(Vector3(0, 0, 0), 10, Vector3(0.5f, 0.3f, 0.7f))));

	setProjectionMatrix(Matrix4::Perspective(1.0f, -100.0f, 1280.0f / 720.0f, 45.0f));
	setCameraMatrix(Matrix4::Translation(Vector3(0, 0, -10.0f)));
	*cameraPos = Vector3(0.0f, 0.0f, -20.0f);
	*cameraRotation = Vector3(0, 180, 0);
	track = new Track();
	lastPieceDeleted = -1;
	lastPieceAdded = -1;

	// Load the ship
	Ship *ship = new Ship(Vector3(0, 0, -20.0f), Vector3(0, 0, 0.0f), Vector3(0, 0, 0));
	addEntity(ship, "Ship");

	// Load track pieces
	track->generateStarterPieces();
	std::vector<TrackPiece*> *pieces = track->getTrackPieces();
	for (unsigned i = 0; i < pieces->size(); i++) {
		lastPieceAdded = i;
		addEntity((*pieces)[i], to_string((long long) lastPieceAdded));
	}
	track->loadTrackPiecesFromFile("resources/track/track_pieces.tpc");
	targetSpeed = 500.0f;

	// Set up the simulation
	Simulation::getInstance()->setGravity(Simulation::GRAVITY_ZERO);
	Simulation::getInstance()->setBroadphaseDetector(new ShipBroadDetector());

	// Set the ambience music
	Music *music = Music::getOrCreate("resources/audio/GameAmbience.mp3", "GameMusic");
}

RaceLevel::~RaceLevel(void) {
}

void RaceLevel::onStart() {
	lockMutex();
	Entity *ship = getEntity("Ship");
	ship->getPhysicalBody()->setVelocity(Vector3(0, 0, 0.0f), 1);
	unlockMutex();
	timeRunning = 0;
	distanceTravelled = 0;
	lastDoorset = 0;
	startTime = Simulation::getInstance()->getTimer()->getStartTime();
	canControl = false;
	Simulation::getInstance()->startSimulation();
	GameApp::getInstance()->setGamePaused(false);
	Music *music = (Music*) ResourcesManager::getResource("GameMusic");
	music->play(1000);
	userInterface->startFadeIn(3000);
	Ship::setSideSpeed(50.0f);
}

void RaceLevel::onPause() {
	// Hide all interface items, show pause menu
	unsigned count = userInterface->getItems()->size();
	userInterface->getItem("PauseMenu")->setHidden(false);
	userInterface->getItem("Speedometer")->setHidden(true);
	userInterface->getItem("DistanceMeter")->setHidden(true);
	userInterface->getItem("SpeedIncIndicator")->setHidden(true);
	Music *music = (Music*) ResourcesManager::getResource("GameMusic");
	music->pause();
}

void RaceLevel::onResume() {
	// Hide pause menu, show all hidden items
	unsigned count = userInterface->getItems()->size();
	userInterface->getItem("PauseMenu")->setHidden(true);
	userInterface->getItem("Speedometer")->setHidden(false);
	userInterface->getItem("DistanceMeter")->setHidden(false);
	Music *music = (Music*) ResourcesManager::getResource("GameMusic");
	music->play(0);
}

void  RaceLevel::onFinish() {
	Simulation::getInstance()->stopSimulation();
	Simulation::resetSimulation();
	Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
	music->stop(0);
}

void RaceLevel::onMouseMoved(Vector2 &position, Vector2 &amount) {
	Level::onMouseMoved(position, amount);
}

void RaceLevel::onMouseClick(Uint8 button, Vector2 &position) {
	Level::onMouseClick(button, position);
}

void RaceLevel::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	Level::onMouseDoubleClick(button, position);
}

void RaceLevel::onMouseButtonDown(Uint8 button, Vector2 &position) {
	Level::onMouseButtonDown(button, position);
}

void RaceLevel::onMouseButtonUp(Uint8 button, Vector2 &position) {
	Level::onMouseButtonUp(button, position);
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void RaceLevel::onMouseWheelScroll(int amount) {
	Level::onMouseWheelScroll(amount);
}

void RaceLevel::onKeyPress(SDL_Keysym key) {
	if (canControl) {
		Level::onKeyPress(key);
	}
}

void RaceLevel::onKeyDown(SDL_Keysym key) {
	if (canControl) {
		Level::onKeyDown(key);
	}
}

void RaceLevel::onKeyUp(SDL_Keysym key) {
	if (canControl) {
		Level::onKeyUp(key);
	}
}

void RaceLevel::processLevelTick(unsigned int millisElapsed) {
	if (!GameApp::getInstance()->isGamePaused()) {
		timeRunning += millisElapsed;
	}
	// Check state changing events
	double timeNow = Simulation::getInstance()->getTimer()->getLastTick();
	if (timeRunning <= 6500) {
		TextItem *centerText = (TextItem*) userInterface->getItem("CenterText");
		if (timeRunning >= 2000 && centerText->getText() == "GET READY") {
			centerText->setText("3");
			Sound::playSfx("RaceStartSfx", 128);
		}
		if (timeRunning >= 3000 && centerText->getText() == "3") {
			centerText->setText("2");
		}
		if (timeRunning >= 4000 && centerText->getText() == "2") {
			centerText->setText("1");
		}
		if (timeRunning >= 5000 && canControl == false) {
			canControl = true;
			centerText->setText("GO!");
		}
		if (timeRunning >= 6000 && centerText->getText() == "GO!") {
			centerText->setHidden(true);
		}
	}

	// Update distance meter
	if (canControl && !GameApp::getInstance()->isGamePaused()) {
		distanceTravelled += (targetSpeed * (millisElapsed / 10000.0f));
	}
	TextItem *distMeter = (TextItem*) userInterface->getItem("DistanceMeter");
	char distBuffer[100];
	sprintf(distBuffer, "Score: %05dm", (int) (distanceTravelled));
	distMeter->setColour(Colour(0xFFFFFFFF));
	distMeter->setText(distBuffer);

	Level::processLevelTick(millisElapsed);

	lockMutex();
	Ship *ship = (Ship*) getEntity("Ship");
	Vector3 shipPos = Vector3(*(ship->getPhysicalBody()->getPosition()));
	Vector3 shipVel = ship->getPhysicalBody()->getVelocity(5.0f);
	Vector3 shipAccel = ship->getPhysicalBody()->getAcceleration();

	// Update Speedometer
	TextItem *speedometer = (TextItem*) userInterface->getItem("Speedometer");
	char buffer[50];
	sprintf(buffer, "%03d km/h", (int) (shipVel.z));
	speedometer->setColour(Colour(0xFFFFFFFF));
	speedometer->setText(buffer);


	// Update ship
	float dragMod = (1.0f + ship->getPhysicalBody()->getdragFactor());
	float accel = min((targetSpeed - shipVel.z) * dragMod, 10.0f);
	if (!canControl) {
		ship->setRoll(0);
		ship->setPitch(0);
		ship->setYaw(0);
		ship->getPhysicalBody()->setAcceleration(Vector3(0, 0, accel));
	} else {
		ship->getPhysicalBody()->setAcceleration(Vector3(shipAccel.x, shipAccel.y, accel));
	}
	if ((timeRunning % 15000) <= millisElapsed && canControl && timeRunning > 1000) {
		// Each 15 seconds increase the ship's speed and also the door closing speed, so the ship will not outrun the doors
		int speedChangeCount = (int) (timeRunning / 15000.0f);
		targetSpeed = 500.0f + (25.0f * speedChangeCount);
		Ship::setSideSpeed(50.0f + (5.0f * speedChangeCount));
		Door::setClosingSpeed(15.0f + (0.75f * speedChangeCount));
		Sound::playSfx("SpeedIncreasedSfx", 64);
		userInterface->getItem("SpeedIncIndicator")->setHidden(false);
	}
	if ((timeRunning % 15500) <= millisElapsed && canControl) {
		userInterface->getItem("SpeedIncIndicator")->setHidden(true);
	}
	// Check for collision, if it happens, game over
	if (ship->getPhysicalBody()->getUnprocessedCollisions() > 0) {
		userInterface->getItem("GameOverOverlay")->setHidden(false);
		((GameOverOverlay*) userInterface->getItem("GameOverOverlay"))->setDistance(distanceTravelled);
		userInterface->getItem("Speedometer")->setHidden(true);
		userInterface->getItem("DistanceMeter")->setHidden(true);
		targetSpeed = 0;
		Ship::setSideSpeed(0);
		ship->getPhysicalBody()->setVelocity(Vector3(0, 0, 0), 0);
		if (canControl) {
			Sound::playSfx("GameOverSfx", 128);
			Sound::playSfx("ExplosionSfx", 128);
			Music *music = (Music*) ResourcesManager::getResource("MenuMusic");
			music->stop(1000);
			int leaderboardPos = Leaderboard::getInstance()->pushNewScore(distanceTravelled);
			((GameOverOverlay*) userInterface->getItem("GameOverOverlay"))->setLeaderboardPos(leaderboardPos);
		}
		canControl = false;
	}

	// Update race track
	TrackPiece *newPiece = track->generateNextPiece(shipPos);
	if (newPiece != NULL) {
		lastPieceAdded++;
		addEntity(newPiece, to_string((long long) lastPieceAdded));
	}
	TrackPiece *deletedPiece = track->deleteOldPiece(shipPos);
	if (deletedPiece != NULL) {
		lastPieceDeleted++;
		TrackPiece *removed = (TrackPiece*) getEntity(to_string((long long) lastPieceDeleted));
		removeEntity(to_string((long long) lastPieceDeleted));
		delete removed;
	}

	// Check if player passed another doorset
	std::vector<TrackPiece*> *trackPieces = track->getTrackPieces();
	for (unsigned i = 0; i < trackPieces->size(); i++) {
		TrackPiece *piece = (*trackPieces)[i];
		std::vector<DoorSet*> *doorsets = piece->getDoorSets();
		for (unsigned j = 0; j < doorsets->size(); j++) {
			DoorSet *doorSet = (*doorsets)[j];
			int doorSetPosZ = piece->getPhysicalBody()->getPosition()->z - 45.0f + doorSet->getPosition();
			if (shipPos.z >doorSetPosZ && doorSetPosZ > lastDoorset && doorSet->getDoors()->size() > 0) {
				// Player passed through a door
				Sound::playSfx("DoorPassedSfx", 32);
				lastDoorset = doorSetPosZ;
				distanceTravelled += 100; // Award him some points
			}
		}
	}

	unlockMutex();
}

void RaceLevel::calculateCameraMatrix() {
	Matrix4 rotationMatrix = Matrix4::Rotation(cameraRotation->x, Vector3(1, 0, 0)) * Matrix4::Rotation(cameraRotation->y, Vector3(0, 1, 0)) * Matrix4::Rotation(cameraRotation->z, Vector3(0, 0, 1));
	*cameraMatrix = Matrix4::Translation(Vector3(0, -1.0f, -10.0f)) * rotationMatrix * Matrix4::Translation(-(*cameraPos));
}