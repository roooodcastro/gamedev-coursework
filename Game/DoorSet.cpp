#include "DoorSet.h"

DoorSet::DoorSet(void) {
	doors = new std::vector<Door*>();
	trackPiece = NULL;
	position = 0;
}

DoorSet::DoorSet(const DoorSet &copy) {
	doors = new std::vector<Door*>(*(copy.doors));
	trackPiece = new TrackPiece(*(copy.trackPiece));
	position = copy.position;
}

DoorSet::DoorSet(TrackPiece *trackPiece, float position) {
	doors = new std::vector<Door*>();
	this->trackPiece = trackPiece;
	this->position = position;
	this->createDoors("");
}

DoorSet::~DoorSet(void) {
	for (unsigned i = 0; i < 6; i++) {
		(*doors)[i]->setDoorSet(NULL);
		delete (*doors)[i];
	}
	delete doors;
}

void DoorSet::createDoors(const char *pattern) {
	// TODO: read the pattern to create the doors in the specified pattern loaded from file

	for (unsigned i = 0; i < 6; i++) {
		Door *door = new Door(this, i, 0.15f);
		trackPiece->addChild(door);
		doors->emplace_back(door);
	}
}

void DoorSet::closeDoors() {
	for (unsigned i = 0; i < doors->size(); i++) {
		Door *door = (*doors)[i];
		if (door->getOpenness() > door->getTargetOpenness()) {
			door->setActive(true);
		}
	}
}