#pragma once

#include "PanelItem.h"
#include "ImageItem.h"
#include "TextItem.h"

class PauseMenu : public PanelItem {

public:
	PauseMenu(void);
	PauseMenu(const PauseMenu &copy);
	virtual ~PauseMenu(void);

	/* Mouse events */
	virtual void onMouseMoved(Vector2 &position, Vector2 &amount) {} // Will fire every time the mouse moves
	virtual void onMouseClick(Uint8 button, Vector2 &position) {} // Will fire once a mouse button is released
	virtual void onMouseDoubleClick(Uint8 button, Vector2 &position) {} // Will fire on a double click
	virtual void onMouseButtonDown(Uint8 button, Vector2 &position) {} // Will fire in every tick that a button is down
	virtual void onMouseButtonUp(Uint8 button, Vector2 &position) {} // Will fire every time a mouse button is released
	virtual void onMouseWheelScroll(int amount) {} // Will fire every time the mouse wheel scrolls

	/* Keyboard events */
	virtual void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	virtual void onKeyDown(SDL_Keysym key) {} // Will fire in every tick that a key is down
	virtual void onKeyUp(SDL_Keysym key) {} // Will fire every time a key is released

protected:

	TextItem *pauseText;
};