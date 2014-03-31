/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This class contains the track pieces for the race track. It also detects
 * where the ship is, and generates more track pieces to place ahead of the ship, indefinitely.
 * When the ship passes though a piece, it gets deletes when it reaches a certain distance behind
 * the ship, to conserve memory. Each new track piece is randomized, generating a new piece
 * design for every new track piece. This process continues until the player crashes and dies.
 */

#pragma once

#include <vector>

/* Struct to store doorset configuration for the track pieces */
struct DoorsetConfiguration {
	float position;
	float openness[6];
	DoorsetConfiguration(float position) : position(position) {}
};

/* Struct to hold track piece configurations to be added randomly to the track */
struct TrackPieceConfiguration {
	int difficulty;
	std::vector<DoorsetConfiguration*> doorsets;
	TrackPieceConfiguration(int difficulty) : difficulty(difficulty) {}
};

#include "TrackPiece.h"
#include "Vector3.h"

class TrackPiece;

class Track {
public:

	Track(void);
	~Track(void);

	std::vector<TrackPiece*> *getTrackPieces() { return pieces; }

	/*
	 * This method checks if the player reached a certain distance from
	 * the last piece loaded, and if he is close enough to that last
	 * track piece, generates a new one ahead of it, to keep the game
	 * going, until the player dies.
	 */
	TrackPiece *generateNextPiece(Vector3 shipPos);

	/*
	 * This method checks if the player already passed through the first
	 * track piece. If so, it will delete that piece, releasing its memory
	 * so the game can create more pieces ahead.
	 */
	TrackPiece *deleteOldPiece(Vector3 shipPos);

	/*
	 * This method generates the first set of track pieces, which are just
	 * doorless track, to allow the player to get used to the environment.
	 */
	void generateStarterPieces();

	/*
	 * Reads a .tpc (TrackPieCe) file to load all the track piece configurations that waill be used in the game.
	 * Doing this when track pieces are already loaded will append the new pieces to the list, even if they are duplicates
	 */
	void loadTrackPiecesFromFile(const char *fileName);

protected:

	std::vector<TrackPiece*> *pieces;

	std::vector<TrackPieceConfiguration*> *loadedConfigurations;
};