#include "UserInterface.h"
#include "TextItem.h"

UserInterface::UserInterface(void) {
	items = new std::vector<InterfaceItem*>();
	showFpsCounter = false;
	fpsCounter = new TextItem(Vector2(20, 10), 0, "0 FPS", 20);
}

UserInterface::UserInterface(const UserInterface &copy) {
	items = new std::vector<InterfaceItem*>();
	*items = *(copy.items);
	showFpsCounter = copy.showFpsCounter;
	fpsCounter = new TextItem(*((TextItem*) copy.fpsCounter));
}


UserInterface::~UserInterface(void) {
	if (items) {
		items->clear();
		delete items;
	}
	delete fpsCounter;
}

void UserInterface::onMouseMoved(Vector2 &position, Vector2 &amount) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseMoved(position, amount);
	}
}

void UserInterface::onMouseClick(Uint8 button, Vector2 &position) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseClick(button, position);
	}
}

void UserInterface::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseDoubleClick(button, position);
	}
}

void UserInterface::onMouseButtonDown(Uint8 button, Vector2 &position) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseButtonDown(button, position);
	}
}

void UserInterface::onMouseButtonUp(Uint8 button, Vector2 &position) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseButtonUp(button, position);
	}
}

void UserInterface::onMouseWheelScroll(int amount) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onMouseWheelScroll(amount);
	}
}

void UserInterface::onKeyPress(SDL_Keysym key) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onKeyPress(key);
	}
}

void UserInterface::onKeyDown(SDL_Keysym key) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onKeyDown(key);
	}
}

void UserInterface::onKeyUp(SDL_Keysym key) {
	// The f3 key will be our debug key, showing debug info on screen
	if (key.sym == SDLK_F3) {
		setShowFpsCounter(!showFpsCounter);
	}
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->onKeyUp(key);
	}
}

void UserInterface::setShowFpsCounter(bool showFpsCounter) {
	this->showFpsCounter = showFpsCounter;
	//((TextItem*) fpsCounter)->setText("0 FPS");
}

void UserInterface::update(unsigned millisElapsed) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->update(millisElapsed);
	}
	if (GameApp::getInstance()->getFramesCount() % (GameApp::TARGET_FPS / 10) == 0) {
		int fps = GameApp::getInstance()->getFps();
		// Don't ask me why I had to put "long long" there, it's just the way it is...
		((TextItem*) fpsCounter)->setText(to_string((long long) fps) + " FPS");
		fpsCounter->update(millisElapsed);
	}
}

void UserInterface::draw(unsigned millisElapsed) {
	for (std::vector<InterfaceItem*>::iterator it = items->begin(); it != items->end(); ++it) {
		(*it)->draw(millisElapsed);
	}
	if (showFpsCounter) {
		fpsCounter->draw(millisElapsed);
	}
}

void UserInterface::addItem(InterfaceItem *item) {
	if (item) {
		// If we don't have space to store the item, make some!
		// I set this if to >= to always have an empty space in the array, just in case
		if ((items->size() + 1) >= items->capacity()) {
			items->reserve(items->capacity() + 10);
		}
		items->emplace_back(item);
	}
}

bool UserInterface::removeItem(InterfaceItem *item) {
	try {
		for (auto it = items->begin(); it != items->end();) {
			if((*it) == item) {
				// Found the entity. Remove it from vector and return true
				items->erase(it);
				return true;
			} else { ++it; }
		}
		// Didn't find the entity in the vector
		return false;
	} catch (int &) {
		// An error occurred while trying toremove the entity
		return false;
	}
}

UserInterface &UserInterface::operator=(const UserInterface &other) {
	this->items = other.items;
	this->showFpsCounter = other.showFpsCounter;
	this->fpsCounter = other.fpsCounter;
	return *this;
}