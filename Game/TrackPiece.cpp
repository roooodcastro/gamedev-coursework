#include "TrackPiece.h"

TrackPiece::TrackPiece(void) : Entity() {
	this->model = Model::getOrCreate("TRACK_PIECE_1", "resources/models/TrackPiece.mdl");
	this->getPhysicalBody()->setRotation(Vector3(0, 90, 0));
	this->getPhysicalBody()->setScale(Vector3(90, 30, 30));
	this->doorSets = new std::vector<DoorSet*>();
	this->generateDoorSets();
}

TrackPiece::TrackPiece(Vector3 &position) : Entity() {
	this->model = Model::getOrCreate("TRACK_PIECE_1", "resources/models/TrackPiece.mdl");
	this->getPhysicalBody()->setRotation(Vector3(0, 90, 0));
	this->getPhysicalBody()->setScale(Vector3(90, 30, 30));
	this->getPhysicalBody()->setPosition(position);
	this->getPhysicalBody()->setVelocity(Vector3(0, 0, 0), 1);
	this->doorSets = new std::vector<DoorSet*>();
	this->generateDoorSets();
}

TrackPiece::TrackPiece(const TrackPiece &copy) : Entity(copy) {
	this->doorSets = new std::vector<DoorSet*>(*(copy.doorSets));
}

TrackPiece::~TrackPiece(void) {
	for (unsigned i = 0; i < doorSets->size(); i++) {
		(*doorSets)[i]->setTrackPiece(NULL);
		delete (*doorSets)[i];
	}
	delete doorSets;
}

TrackPiece &TrackPiece::operator=(const TrackPiece &other) {
	static_cast<Entity&>(*this) = other;
	*(this->doorSets) = *(other.doorSets);
	return *this;
}

void TrackPiece::generateDoorSets() {
	DoorSet *doorSet = new DoorSet(this, 80);
	doorSets->emplace_back(doorSet);
}

void TrackPiece::update(unsigned millisElapsed) {
	Entity::update(millisElapsed);
	for (unsigned i = 0; i < doorSets->size(); i++) {
		DoorSet *doorSet = (*doorSets)[i];
		Vector3 shipPos = Vector3(*(GameApp::getInstance()->getCurrentLevel()->getEntity("Ship")->getPhysicalBody()->getPosition()));
		float doorZ = getPhysicalBody()->getPosition()->z + doorSet->getPosition();
		if (doorZ - shipPos.z <= 540.0f) {
			doorSet->closeDoors();
		}
	}
}