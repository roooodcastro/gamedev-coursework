#pragma once

#include "Matrix4.h"
#include "GameApp.h"
#include "Vector2.h"
#include "Texture.h"

enum InterfaceItemType {
	ITEM_BUTTON, // A clickable button
	ITEM_IMAGE, // Just a image
	ITEM_3D_FRAME, // A frame that can render a perspective 3D model into it
	ITEM_PANEL, // A panel that can hold child interface items
};

class Texture;

class InterfaceItem {
public:
	InterfaceItem(void);
	InterfaceItem(InterfaceItemType type);
	InterfaceItem(InterfaceItemType type, Vector2 &position, float rotation, Vector2 &size);
	virtual ~InterfaceItem(void);

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

	/* Calculates is the mouse position provided is inside this interface item */
	bool isMouseHovering(Vector2 &mousePos);

	/* General getters and setters */
	InterfaceItemType getType() { return type; }
	virtual void setTexture(Texture *texture) { this->texture = texture; }
	virtual Texture *getTexture() { return texture; }

	void setPosition(Vector2 &position) { this->position = &position; }
	Vector2 getPosition() { return *position; }
	void setRotation(float rotation) { this->rotation = rotation; }
	float getRotation() { return rotation; }
	void setSize(Vector2 &size) { this->size = &size; }
	Vector2 getSize() { return *size; }
	void setModelMatrix(Matrix4 &modelMatrix) { this->modelMatrix = modelMatrix; }
	Matrix4 getModelMatrix() { return modelMatrix; }

	const static float SIZE_NO_RESIZE;

protected:

	/* Returns the actual size, calculating the size in case it is set to some constant, like SIZE_NO_RESIZE */
	Vector2 getRealSize();

	/* The screen position of this item */
	Vector2 *position;
	/* The screen rotation of this item */
	float rotation;
	/* The size in pixels of this item */
	Vector2 *size;
	/* The type of this item. Different item types will have different behaviour */
	InterfaceItemType type;
	/* The texture to be used to render this item */
	Texture *texture;
	/* The model matrix to be applied to the texture in order to render it. Internal use only */
	Matrix4 modelMatrix;
};