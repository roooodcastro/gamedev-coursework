#include "TrackPiece.h"

TrackPiece::TrackPiece(void) : Entity() {
	this->model = Model::loadObjFile("resources/models/TrackPiece.mdl");
	this->setRotation(Vector3(0, 90, 0));
	this->setScale(Vector3(90, 30, 30));
	this->doorSets = new std::vector<DoorSet*>();
	this->generateDoorSets();
}

TrackPiece::TrackPiece(Vector3 &position) : Entity() {
	this->model = Model::loadObjFile("resources/models/TrackPiece.mdl");
	this->setRotation(Vector3(0, 90, 0));
	this->setScale(Vector3(90, 30, 30));
	this->setPosition(position);
	this->doorSets = new std::vector<DoorSet*>();
	this->generateDoorSets();
}

TrackPiece::TrackPiece(const TrackPiece &copy) : Entity(copy) {
	this->doorSets = new std::vector<DoorSet*>(*(copy.doorSets));
}

TrackPiece::~TrackPiece(void) {
	for (unsigned i = 0; i < doorSets->size(); i++) {
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
	doorSets->push_back(doorSet);
}