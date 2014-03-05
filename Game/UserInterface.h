#pragma once

#include <stdlib.h>
#include <vector>
#include "InterfaceItem.h"

class InterfaceItem;
class Shader;

class UserInterface {
public:

	UserInterface(void);
	UserInterface(const UserInterface &copy);
	~UserInterface(void);

	/* Mouse events */
	void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
	void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
	void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
	void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
	void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
	void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls
	/* Keyboard events */
	void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
	void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

	void update(unsigned millisElapsed);
	void draw(unsigned millisElapsed);

	/* General getters and setters */
	void addItem(InterfaceItem *item);
	bool removeItem(InterfaceItem *item);
	std::vector<InterfaceItem*> *getItems() { return items; }
	void setShowFpsCounter(bool showFps);
	bool isShowFpsCounter() { return showFpsCounter; }
	Shader *getInterfaceShader() { return interfaceShader; }

	UserInterface &operator=(const UserInterface &other);

protected:

	/*
	 * The list of interface items included in this interface
	 */
	std::vector<InterfaceItem*> *items;

	/* Debug tool to show fps counter on the top left corner of screen */
	bool showFpsCounter;
	InterfaceItem *fpsCounter;
	Shader *interfaceShader; // The default shader to be used on the interface. Plain simple shader.
};