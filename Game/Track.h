/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is the main race track of the game. It reads track piece configurations from files and randomly
 * set them up one after the other, offering the player a dynamic, random and challenging track. The track pieces used
 * will vary accordingly with the distance ran, the farther the player goes, the harder it gets and harder track pieces
 * will be chosen much more frequently than easier ones. For each track piece the player passes, an old one will be
 * unloaded behind him, and a new one will be loaded ahead, in a way that the player won't notice that the track isn't
 * actually infinite.
 * 
 */

#pragma once

#include <vector>
#include "TrackPiece.h"

class Track {
public:

	Track(void);
	~Track(void);

protected:

	std::vector<TrackPiece*> *pieces;
};