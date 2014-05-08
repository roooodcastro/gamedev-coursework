/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is the screen that shows the leaderboard of the game. It displays the
 * five top scores recorded on the config file.
 */

#pragma once

#include "Level.h"
#include "ImageItem.h"
#include "TextItem.h"
#include "ButtonItem.h"
#include "GameMenu.h"
#include "Music.h"
#include "Sound.h"

class LeaderboardScreen : public Level {
public:

	LeaderboardScreen(void);
	~LeaderboardScreen(void);

	/* Level related events */
	virtual void onStart(); // Will fire when the current level of the game is switched to this
	virtual void onPause(); // Will fire when the gmae pauses
	virtual void onResume(); // Will fire when the game restarts from a pause state
	virtual void onFinish(); // Will fire when the current level of the game is switched to another one

	virtual void processLevelTick(unsigned int millisElapsed);

	void setActionAfterFade(int action) { this->actionAfterFade = action; }

protected:

	/* Internal variable to tell the menu what to do after the fadeOut has finished. Defaults to 0 */
	int actionAfterFade;
};