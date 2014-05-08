/*
 * Author: Rodrigo Castro
 *
 * Description: This is the first screen to be shown to the player. It loads all assets the game will use
 * and shows the name and logo of the game. It will be switched with the main menu automatically, after
 * everything has benn loaded and a minimum time has passed
 */

#pragma once

#include "Level.h"
#include "ImageItem.h"
#include "TextItem.h"
#include "ButtonItem.h"
#include "GameMenu.h"
#include "ConfigurationManager.h"

class SplashScreen : public Level {
public:

	SplashScreen(void);
	~SplashScreen(void);

	/* Level related events */
	virtual void onStart(); // Will fire when the current level of the game is switched to this
	virtual void onPause(); // Will fire when the gmae pauses
	virtual void onResume(); // Will fire when the game restarts from a pause state
	virtual void onFinish(); // Will fire when the current level of the game is switched to another one

	virtual void processLevelTick(unsigned int millisElapsed);

private:

	/* The total time that the ship has been flying */
	unsigned timeRunning;
	/* Counter for the loading text dot animation */
	int lastAnim;

};

