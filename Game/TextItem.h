#pragma once

#include "InterfaceItem.h"

class TextItem : public InterfaceItem {
public:

	TextItem(void);
	TextItem(string text, int fontSize);
	TextItem(Vector2 &position, float rotation, string text, int fontSize);
	TextItem(Vector2 &position, float rotation, Vector2 &size);
	TextItem(Vector2 &position, float rotation, Vector2 &size, string text, int fontSize);
	virtual ~TextItem(void);

	/* Mouse events */
	virtual void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
	virtual void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
	virtual void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
	virtual void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
	virtual void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
	virtual void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls

	/* Keyboard events */
	virtual void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	virtual void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
	virtual void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

	/* General getters and setters */
	void setText(string text);
	string getText() { return text; }
	void setColour(Colour &colour);
	Colour *getColour() { return colour; }
	void setFont(string font);
	string getFont() { return font; }
	void setFontSize(int fontSize);
	int getFontSize() { return fontSize; }

	/* We set these to Arial 16 */
	static const string defaultFont;
	static const int defaultFontSize;

protected:

	/*
	 * This function will be called everytime an attribute of this text is modified,
	 * because as the text is rendered as a texture, we can't just go and edit it
	 * directly, instead we always have to rebuild it, no matter how small the change is.
	 */
	void reloadText();

	string text; // The text label, what will be written
	string font; // The text font filename (including the path)
	int fontSize; // The font size, in points
	Colour *colour; // The colour of the text. If a background colour is needed, an ImageItem should be put behind this text with the colour

};