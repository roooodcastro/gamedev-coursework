#include "RaceLevel.h"

RaceLevel::RaceLevel(void) : Level(LEVEL_GAME, UserInterface()) {
	userInterface->addItem(new PauseMenu(), "PauseMenu");
	int windowWidth = GameApp::getInstance()->getWindowWidth();
	int windowHeight = GameApp::getInstance()->getWindowHeight();
	TextItem *speedometer = new TextItem(Vector2(windowWidth - 160.0f, windowHeight - 60.0f), 0, "000 km/h", 32);
	speedometer->setColour(Colour(0, 0, 0, 255));
	userInterface->addItem(speedometer, "Speedometer");

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
}

void RaceLevel::onPause() {
	// Hide all interface items, show pause menu
	unsigned count = userInterface->getItems()->size();
	userInterface->getItem("PauseMenu")->setHidden(false);
	userInterface->getItem("Speedometer")->setHidden(true);
}

void RaceLevel::onResume() {
	// Hide pause menu, show all hidden items
	unsigned count = userInterface->getItems()->size();
	userInterface->getItem("PauseMenu")->setHidden(true);
	userInterface->getItem("Speedometer")->setHidden(false);
}

void  RaceLevel::onFinish() {
	// Should unload textures and VAOs and set any states or variables that need to be set
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
	Level::onKeyPress(key);
}

void RaceLevel::onKeyDown(SDL_Keysym key) {
	Level::onKeyDown(key);
}

void RaceLevel::onKeyUp(SDL_Keysym key) {
	Level::onKeyUp(key);
}

void RaceLevel::processLevelTick(unsigned int millisElapsed) {
	Level::processLevelTick(millisElapsed);
	lockMutex();
	timeRunning += millisElapsed;
	Entity *ship = getEntity("Ship");
	Vector3 shipPos = Vector3(*(ship->getPhysicalBody()->getPosition()));
	Vector3 shipVel = ship->getPhysicalBody()->getVelocity(5.0f);
	Vector3 shipAccel = ship->getPhysicalBody()->getAcceleration();
	// Update ship
	float dragMod = (1.0f + ship->getPhysicalBody()->getdragFactor());
	float accel = min((targetSpeed - shipVel.z) * dragMod, 10.0f);
	ship->getPhysicalBody()->setAcceleration(Vector3(shipAccel.x, shipAccel.y, accel));
	if ((timeRunning % 15000) <= millisElapsed) {
		// Each 15 seconds increase the ship's speed and also the door closing speed, so the ship will not outrun the doors
		std::cout << millisElapsed << std::endl;
		std::cout << (int) (timeRunning / 15000) << std::endl;
		int speedChangeCount = (int) (timeRunning / 15000.0f);
		targetSpeed = 500.0f + (25.0f * speedChangeCount);
		Ship::setSideSpeed(50.0f + (5.0f * speedChangeCount));
		Door::setClosingSpeed(15.0f + (0.75f * speedChangeCount));

	}
	distanceTravelled += (targetSpeed * (millisElapsed / 1000.0f));
	//std::cout << distanceTravelled << std::endl;

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
	TextItem *speedometer = (TextItem*) userInterface->getItem("Speedometer");
	char buffer[50];
	sprintf(buffer, "%03d km/h", (int) (shipVel.z));
	speedometer->setText(buffer);
	unlockMutex();
}

void RaceLevel::calculateCameraMatrix() {
	Matrix4 rotationMatrix = Matrix4::Rotation(cameraRotation->x, Vector3(1, 0, 0)) * Matrix4::Rotation(cameraRotation->y, Vector3(0, 1, 0)) * Matrix4::Rotation(cameraRotation->z, Vector3(0, 0, 1));
	*cameraMatrix = Matrix4::Translation(Vector3(0, -1.0f, -10.0f)) * rotationMatrix * Matrix4::Translation(-(*cameraPos));
}