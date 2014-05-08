/*
 * Author: Rodrigo Castro
 *
 * Description: This is the main menu of the game. It will contain buttons to
 * access the game, leaderboard, help and also quit the game.
 */

#pragma once

#include "Level.h"
#include "ImageItem.h"
#include "TextItem.h"
#include "ButtonItem.h"
#include "RaceLevel.h"
#include "HelpScreen.h"
#include "Music.h"
#include "Sound.h"
#include "ConfigurationManager.h"
#include "LeaderboardScreen.h"

class GameMenu : public Level {
public:

	GameMenu(void);
	~GameMenu(void);

	/* Level related events */
	virtual void onStart(); // Will fire when the current level of the game is switched to this
	virtual void onPause(); // Will fire when the gmae pauses
	virtual void onResume(); // Will fire when the game restarts from a pause state
	virtual void onFinish(); // Will fire when the current level of the game is switched to another one

	virtual void processLevelTick(unsigned int millisElapsed);

	void setActionAfterFade(int action) { this->actionAfterFade = action; }

private:

	/* Internal variable to tell the menu what to do after the fadeOut has finished. Defaults to 0 */
	int actionAfterFade;
};

