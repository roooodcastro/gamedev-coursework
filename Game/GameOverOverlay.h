

#pragma once

#include "PanelItem.h"
#include "ImageItem.h"
#include "TextItem.h"
#include "ButtonItem.h"
#include "RaceLevel.h"
#include "GameMenu.h"
#include "LeaderboardScreen.h"

class GameOverOverlay : public PanelItem {

public:
	GameOverOverlay(void);
	~GameOverOverlay(void);

	/* Keyboard events */
	virtual void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	virtual void onKeyDown(SDL_Keysym key) {} // Will fire in every tick that a key is down
	virtual void onKeyUp(SDL_Keysym key) {} // Will fire every time a key is released

	virtual void update(unsigned millisElapsed);

	void setDistance(int distance);

	void setLeaderboardPos(int pos);

protected:

	TextItem *centerText;
	TextItem *scoreText;
	ButtonItem *restartButton;
	ButtonItem *leaderboardButton;
	ButtonItem *menuButton;
	ButtonItem *quitButton;

	/* The score of the player */
	int distance;
	/* The position of the leaderboard that the player achieved */
	int leaderboardPos;
};