#include "TrackPiece.h"

TrackPiece::TrackPiece(void) : Entity() {
	this->model = Model::getOrCreate("TRACK_PIECE_1", "resources/models/TrackPiece.mdl");
	this->getPhysicalBody()->setRotation(Vector3(0, 0, 0));
	this->getPhysicalBody()->setScale(Vector3(30, 30, 90));
	physicalBody->setCanMove(false);
	physicalBody->setElasticity(0.0f);
	this->physicalBody->setCollisionGroup(2);
	this->getPhysicalBody()->addCollisionBody(new CollisionBody(physicalBody, Vector3(0, 0, 0), 23.0f, 90.0f, Vector3(0, 0, 1)));
	this->doorSets = new std::vector<DoorSet*>();
	this->generateDoorSets();
}

TrackPiece::TrackPiece(Vector3 &position) : Entity() {
	this->model = Model::getOrCreate("TRACK_PIECE_1", "resources/models/TrackPiece.mdl");
	this->getPhysicalBody()->setRotation(Vector3(0, 0, 0));
	this->getPhysicalBody()->setScale(Vector3(30, 30, 90));
	this->getPhysicalBody()->setPosition(position);
	this->getPhysicalBody()->setVelocity(Vector3(0, 0, 0), 1);
	physicalBody->setCanMove(false);
	physicalBody->setElasticity(0.0f);
	this->physicalBody->setCollisionGroup(2);
	this->getPhysicalBody()->addCollisionBody(new CollisionBody(physicalBody, Vector3(0, 0, 0), 23.0f, 90.0f, Vector3(0, 0, 1)));
	this->doorSets = new std::vector<DoorSet*>();
	this->generateDoorSets();
}

TrackPiece::TrackPiece(Vector3 &position, TrackPieceConfiguration *configuration) : Entity() {
	this->model = Model::getOrCreate("TRACK_PIECE_1", "resources/models/TrackPiece.mdl");
	this->getPhysicalBody()->setRotation(Vector3(0, 0, 0));
	this->getPhysicalBody()->setScale(Vector3(30, 30, 90));
	this->getPhysicalBody()->setPosition(position);
	this->getPhysicalBody()->setVelocity(Vector3(0, 0, 0), 1);
	physicalBody->setCanMove(false);
	physicalBody->setElasticity(0.0f);
	this->physicalBody->setCollisionGroup(2);
	this->getPhysicalBody()->addCollisionBody(new CollisionBody(physicalBody, Vector3(0, 0, 0), 23.0f, 90.0f, Vector3(0, 0, 1)));
	this->doorSets = new std::vector<DoorSet*>();
	for (unsigned i = 0; i < configuration->doorsets.size(); i++) {
		DoorsetConfiguration *doorSetConfig = configuration->doorsets[i];
		DoorSet *doorSet = new DoorSet(this, doorSetConfig->position);
		std::vector<Door*> *doors = doorSet->getDoors();
		for (int i = 0; i < 6; i++) {
			Door *door = new Door(doorSet, i, doorSetConfig->openness[i]);
			doors->emplace_back(door);
			addChild(door);
		}
		doorSets->emplace_back(doorSet);
	}

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