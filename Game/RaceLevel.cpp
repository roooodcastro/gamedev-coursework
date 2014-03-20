#include "RaceLevel.h"

RaceLevel::RaceLevel(void) : Level(LEVEL_GAME, UserInterface()) {
	userInterface->addItem(new PauseMenu());
	addLightSource(*(new Light(Vector3(0, 52, 0), 20, Vector3(1.0f, 1.0f, 1.0f))));
	addLightSource(*(new Light(Vector3(0, -52, 0), 20, Vector3(1.0f, 1.0f, 1.0f))));
	setProjectionMatrix(Matrix4::Perspective(1.0f, -100.0f, 1280.0f / 720.0f, 45.0f));
	setCameraMatrix(Matrix4::Translation(Vector3(0, 0, -10.0f)));
	*cameraPos = Vector3(0.0f, 0.0f, -20.0f);
	*cameraRotation = Vector3(0, 180, 0);
	track = new Track();
	lastPieceDeleted = -1;
	lastPieceAdded = -1;
}

RaceLevel::~RaceLevel(void) {
}

void RaceLevel::onStart() {
	// Load the ship
	Ship *ship = new Ship(Vector3(0, 0, -20.0f), Vector3(0, 0, 0.0f), Vector3(0, 0, 0));
	addEntity(ship, "Ship");

	track->generateStarterPieces();
	std::vector<TrackPiece*> *pieces = track->getTrackPieces();
	for (unsigned i = 0; i < pieces->size(); i++) {
		lastPieceAdded = i;
		addEntity((*pieces)[i], to_string((long long) lastPieceAdded));
	}
}

void RaceLevel::onPause() {
	// Hide all interface items, show pause menu
	unsigned count = userInterface->getItems()->size();
	for (unsigned i = 0; i < count; i++) {
		userInterface->getItems()->at(i)->setHidden(!userInterface->getItems()->at(i)->isHidden());
	}
}

void RaceLevel::onResume() {
	// Hide pause menu, show all hidden items
	unsigned count = userInterface->getItems()->size();
	for (unsigned i = 0; i < count; i++) {
		userInterface->getItems()->at(i)->setHidden(!userInterface->getItems()->at(i)->isHidden());
	}
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
	Vector3 shipPos = Vector3(*(getEntity("Ship")->getPhysicalBody()->getPosition()));
	TrackPiece *newPiece = track->generateNextPiece(shipPos);
	if (newPiece != NULL) {
		lastPieceAdded++;
		addEntity(newPiece, to_string((long long) lastPieceAdded));
		std::cout << entities->size() << std::endl;
	}
	TrackPiece *deletedPiece = track->deleteOldPiece(shipPos);
	if (deletedPiece != NULL) {
		lastPieceDeleted++;
		TrackPiece *removed = (TrackPiece*) getEntity(to_string((long long) lastPieceDeleted));
		removeEntity(to_string((long long) lastPieceDeleted));
		//delete removed;
	}
}

void RaceLevel::calculateCameraMatrix() {
	Matrix4 rotationMatrix = Matrix4::Rotation(cameraRotation->x, Vector3(1, 0, 0)) * Matrix4::Rotation(cameraRotation->y, Vector3(0, 1, 0)) * Matrix4::Rotation(cameraRotation->z, Vector3(0, 0, 1));
	*cameraMatrix = Matrix4::Translation(Vector3(0, -1.0f, -10.0f)) * rotationMatrix * Matrix4::Translation(-(*cameraPos));
}