#pragma once

#include <vector>
#include <algorithm>
#include "InterfaceItem.h"

/*
 * A panel is an interface item that can have other interface items inside it, just like a panel should.
 * The child items here follow almost the same rules as the entities that are child to another entity,
 * except sor the scaling part. The size of the panel does not influentitate on the size of the child items,
 * because as we're calculating sizes in pixels, this wouldn't make sense. The positioning and rotation,
 * however, are relative to the panel.
 */
class PanelItem : public InterfaceItem {
public:

	PanelItem(void);
	PanelItem(Vector2 &position, float rotation, Vector2 &size);
	virtual ~PanelItem(void);

	/* Gameloop funcions */
	virtual void update(unsigned millisElapsed);
	virtual void draw(unsigned millisElapsed);
	
	/* Mouse events */
	virtual void onMouseMoved(Vector2 &position, Vector2 &amount) = 0; // Will fire every time the mouse moves
	virtual void onMouseClick(Uint8 button, Vector2 &position) = 0; // Will fire once a mouse button is released
	virtual void onMouseDoubleClick(Uint8 button, Vector2 &position) = 0; // Will fire on a double click
	virtual void onMouseButtonDown(Uint8 button, Vector2 &position) = 0; // Will fire in every tick that a button is down
	virtual void onMouseButtonUp(Uint8 button, Vector2 &position) = 0; // Will fire every time a mouse button is released
	virtual void onMouseWheelScroll(int amount) = 0; // Will fire every time the mouse wheel scrolls

	/* Keyboard events */
	virtual void onKeyPress(SDL_Keysym key) = 0; // Will fire every time a key is released
	virtual void onKeyDown(SDL_Keysym key) = 0; // Will fire in every tick that a key is down
	virtual void onKeyUp(SDL_Keysym key) = 0; // Will fire every time a key is released

	/* General getters and setters */
	void addItem(InterfaceItem *item);
	void removeItem(InterfaceItem *item);
	vector<InterfaceItem*> *getInterfaceItems() { return items; }

protected:

	/* A vector containing the interface items inside this panel */
	vector<InterfaceItem*> *items;
};

