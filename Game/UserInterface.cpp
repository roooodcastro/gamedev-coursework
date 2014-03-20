#include "UserInterface.h"
#include "TextItem.h"

UserInterface::UserInterface(void) {
	items = new std::map<std::string, InterfaceItem*>();
	showFpsCounter = false;
	fpsCounter = new TextItem(Vector2(20, 10), 0, "0 FPS", 20);
	interfaceShader = new Shader("shaders/vertUI.glsl", "shaders/fragUI.glsl", "", "", "");
}

UserInterface::UserInterface(const UserInterface &copy) {
	items = new std::map<std::string, InterfaceItem*>(*(copy.items));
	showFpsCounter = copy.showFpsCounter;
	fpsCounter = new TextItem(*((TextItem*) copy.fpsCounter));
	interfaceShader = new Shader(*(copy.interfaceShader));
}

UserInterface::~UserInterface(void) {
	items->clear();
	delete items;
	delete fpsCounter;
	delete interfaceShader;
}

UserInterface &UserInterface::operator=(const UserInterface &other) {
	*(this->items) = *(other.items);
	this->showFpsCounter = other.showFpsCounter;
	*(this->fpsCounter) = *(other.fpsCounter);
	*(this->interfaceShader) = *(other.interfaceShader);
	return *this;
}

void UserInterface::onMouseMoved(Vector2 &position, Vector2 &amount) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		if (!(*it).second->isHidden()) {
			(*it).second->onMouseMoved(position, amount);
		}
	}
}

void UserInterface::onMouseClick(Uint8 button, Vector2 &position) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		if (!(*it).second->isHidden()) {
			(*it).second->onMouseClick(button, position);
		}
	}
}

void UserInterface::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		if (!(*it).second->isHidden()) {
			(*it).second->onMouseDoubleClick(button, position);
		}
	}
}

void UserInterface::onMouseButtonDown(Uint8 button, Vector2 &position) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		if (!(*it).second->isHidden()) {
			(*it).second->onMouseButtonDown(button, position);
		}
	}
}

void UserInterface::onMouseButtonUp(Uint8 button, Vector2 &position) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		if (!(*it).second->isHidden()) {
			(*it).second->onMouseButtonUp(button, position);
		}
	}
}

void UserInterface::onMouseWheelScroll(int amount) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		if (!(*it).second->isHidden()) {
			(*it).second->onMouseWheelScroll(amount);
		}
	}
}

void UserInterface::onKeyPress(SDL_Keysym key) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		if (!(*it).second->isHidden()) {
			(*it).second->onKeyPress(key);
		}
	}
}

void UserInterface::onKeyDown(SDL_Keysym key) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		(*it).second->onKeyDown(key);
	}
}

void UserInterface::onKeyUp(SDL_Keysym key) {
	// The f3 key will be our debug key, showing debug info on screen
	if (key.sym == SDLK_F3) {
		setShowFpsCounter(!showFpsCounter);
	}
	for (auto it = items->begin(); it != items->end(); ++it) {
		if (!(*it).second->isHidden()) {
			(*it).second->onKeyUp(key);
		}
	}
}

void UserInterface::setShowFpsCounter(bool showFpsCounter) {
	this->showFpsCounter = showFpsCounter;
	//((TextItem*) fpsCounter)->setText("0 FPS");
}

void UserInterface::update(unsigned millisElapsed) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		(*it).second->update(millisElapsed);
	}
	if (GameApp::getInstance()->getFramesCount() % (GameApp::TARGET_FPS / 10) == 0) {
		int fps = GameApp::getInstance()->getFps();
		// Don't ask me why I had to put "long long" there, it's just the way it is...
		((TextItem*) fpsCounter)->setText(to_string((long long) fps) + " FPS");
		fpsCounter->update(millisElapsed);
	}
}

void UserInterface::draw(unsigned millisElapsed) {
	for (auto it = items->begin(); it != items->end(); ++it) {
		(*it).second->draw(millisElapsed, interfaceShader->getShaderProgram());
	}
	if (showFpsCounter) {
		fpsCounter->draw(millisElapsed, interfaceShader->getShaderProgram());
	}
}

void UserInterface::addItem(InterfaceItem *item, std::string name) {
	if (item && name != "") {
		items->insert(std::pair<std::string, InterfaceItem*>(name, item));
	}
}

bool UserInterface::removeItem(InterfaceItem *item, std::string name) {
	if (isItemInInterface(name)) {
		return items->erase(name) > 0;
	}
	return false;
}

InterfaceItem *UserInterface::getItem(std::string name) {
	if (isItemInInterface(name)) {
		return items->at(name);
	}
	return NULL;
}

bool UserInterface::isItemInInterface(std::string name) {
	return items->find(name) != items->end();
}