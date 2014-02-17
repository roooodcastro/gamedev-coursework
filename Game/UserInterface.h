#pragma once

#include <stdlib.h>
#include <vector>
#include "InterfaceItem.h"

class InterfaceItem;

class UserInterface {
public:

	UserInterface(void);
	~UserInterface(void);

	// list of interface items, which can be:
	// backgrounds
	// buttons
	// images, or panels, etc
	// 3d panel, to draw perspective opengl stuff
	// create one class for each type of interface item

	// interface will be drawn using orthographic view
	// entities will be drawn using perspective

	/*
	 * The list of interface items included in this interface
	 */
	std::vector<InterfaceItem*> items;
};