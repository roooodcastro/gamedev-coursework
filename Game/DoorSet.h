/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is a set of doors. Each set have 6 doors, that can be opened, closed or partially closed,
 * depending on the specific configuration
 */

#pragma once

#include <vector>
#include "Door.h"
#include "TrackPiece.h"

class TrackPiece;
class Door;

class DoorSet {
public:

	DoorSet(void);
	DoorSet(const DoorSet &copy);
	DoorSet(TrackPiece *trackPiece, float position);
	~DoorSet(void);

	void setTrackPiece(TrackPiece *trackPiece) { this->trackPiece = trackPiece; }
	TrackPiece *getTrackPiece() { return trackPiece; }
	void setPosition(float position) { this->position = position; }
	float getPosition() { return position; }
	std::vector<Door*> *getDoors() { return doors; }
	/* Return true if all doors are fully opened */
	bool areAllDoorsOpened();

	/* Causes the doors in this doorset to start closing, is they are opened */
	void closeDoors();

protected:

	void createDoors(const char *pattern);

	/* The track piece in which this doorset belongs to */
	TrackPiece *trackPiece;

	/* Relative Z position on the track piece */
	float position;

	/* The set of 6 doors */
	std::vector<Door*> *doors;
};