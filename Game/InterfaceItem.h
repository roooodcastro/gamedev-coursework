#pragma once

#include "GameApp.h"
#include "Vector2.h"

enum InterfaceItemType {
	ITEM_BUTTON, // A clickable button
	ITEM_IMAGE, // Just a image
	ITEM_3D_FRAME, // A frame that can render a perspective 3D model into it
};

class InterfaceItem {
public:
	InterfaceItem(void);
	InterfaceItem(InterfaceItemType type);
	~InterfaceItem(void);

	/* Player input methods, should be implemented to define the interface item's behaviour */
	virtual void onMouseHover() = 0;
	virtual void onMouseClick() = 0;
	virtual void onMouseDown() = 0;
	virtual void onMouseUp() = 0;
	virtual void onKeyPress() = 0;
	virtual void onKeyDown() = 0;
	virtual void onKeyUp() = 0;

	InterfaceItemType getType() { return type; }

private:

	/* The screen position of this item */
	Vector2 *position;
	/* The size in pixels of this item */
	Vector2 *size;
	/* The type of this item. Different item types will have different behaviour */
	InterfaceItemType type;
};