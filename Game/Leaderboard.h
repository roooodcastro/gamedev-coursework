/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This class is just a helper to organize the leaderboard. Its purpose is
 * mainly take a score and check if it will get into the leaderboard, and also loading
 * and saving the leaderboard to the system. The leaderboard consists of 5 top scores,
 * and each time a new score higher than one of them is achieved, it will make into the
 * leaderboard, pushing the lower scores out of it.
 */

#pragma once

#include "ConfigurationManager.h"

class Leaderboard {
public:

	~Leaderboard(void);

	static Leaderboard *getInstance();

	/*
	 * Pushes a new score into the leaderboard. If it's higher than at
	 * least the fifth place, it will replace it. It returns the position
	 * on the leaderboard of the score, or 0 if it wasn't high enough.
	 */
	int pushNewScore(int score);

	/* Returns the score of the requested position, ranging from 1 to 5 */
	int getScore(int position);

protected:

	Leaderboard(void);

	static Leaderboard *instance;

	/* Update the leaderboard on the file saved to the disk */
	void updateFile();

	/* Variables to hold the leaderboard values */
	int first;
	int second;
	int third;
	int fourth;
	int fifth;

};