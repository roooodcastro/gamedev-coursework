/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is a track piece, an unit of track that the player must pass through. Each track piece
 * can have a few sets of doors, that are basically triangles that will close in front of the player, that
 * will have to find its way around gaps or open doors. Each set of doors can have up to 6 doors, and a typical
 * easy doorset will have between 1 and 3 doors shut, and the rest will remain open, allowing the player to pass
 * through them. In some harder doorsets, instead of having just open or closed doors, the set will have partially
 * opened doors, that wil offer a very limited space for the player, or even doors with holes in it, for when all
 * six doors are closed.
 *
 */

#pragma once

#include <vector>
#include "DoorSet.h"
#include "Entity.h"

class DoorSet;

class TrackPiece : public Entity {
public:

	TrackPiece(void);
	TrackPiece(const TrackPiece &copy);
	TrackPiece(Vector3 &position);
	~TrackPiece(void);

	TrackPiece &operator=(const TrackPiece &other);

	std::vector<DoorSet*> *getDoorSets() { return doorSets; }

protected:

	/*
	 * This function will randomly choose a doorset pattern from a file and generate it,
	 * creating a number of doorsets with different door configurations
	 */
	void generateDoorSets();

	std::vector<DoorSet*> *doorSets;
};