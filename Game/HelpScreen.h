/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is just a screen showing help and credits information.
 * Contains only the background image and a button to allow the player to
 * go back to the main menu.
 */

#pragma once

#include "Level.h"
#include "ImageItem.h"
#include "TextItem.h"
#include "ButtonItem.h"
#include "GameMenu.h"
#include "Music.h"
#include "Sound.h"

class HelpScreen : public Level {
public:

	HelpScreen(void);
	~HelpScreen(void);

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