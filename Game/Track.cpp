#include "Track.h"

Track::Track(void) {
	pieces = new std::vector<TrackPiece*>();
}

Track::~Track(void) {
	pieces->clear();
	delete pieces;
}

TrackPiece *Track::generateNextPiece(Vector3 shipPos) {
	TrackPiece *last = pieces->at(pieces->size() - 1);
	Vector3 *lastPos = last->getPhysicalBody()->getPosition();
	if (lastPos->z - shipPos.z <= 720.0f) {
		TrackPiece *next = new TrackPiece(Vector3(0, 0, lastPos->z + 180));
		pieces->emplace_back(next);
		return next;
	}
	return NULL;
}

TrackPiece *Track::deleteOldPiece(Vector3 shipPos) {
	TrackPiece *first = (*(pieces->begin()));
	Vector3 *firstPos = first->getPhysicalBody()->getPosition();
	if (firstPos->z - shipPos.z <= -180.0f) {
		pieces->erase(pieces->begin());
		return first;
	}
	return NULL;
}

void Track::generateStarterPieces() {
	for (int i = 0; i < 5; i++) {
		TrackPiece *piece = new TrackPiece(Vector3(0, 0, 180.0f * i));
		pieces->emplace_back(piece);
	}
}