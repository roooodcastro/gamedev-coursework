#include <stdlib.h>
#include <iostream>
#include <SDL.h>
#include "GameApp.h"
#include "SplashScreen.h"
#include "RaceLevel.h"

using namespace std;

int main(int argc, char* argv[]) {

	// Configure game engine for init
	GameApp::initializeContext("GameDev Coursework", 1920, 1080);
	GameApp *gameApp = GameApp::getInstance();
	UserInterface gameInterface = UserInterface();

	// Set the window title and icon
	SDL_Surface *icon = IMG_Load("resources/images/Icon.png");
	SDL_SetWindowIcon(GameApp::getInstance()->getWindow(), icon);
	SDL_SetWindowTitle(GameApp::getInstance()->getWindow(), "heXcape");
	
	// Create a level and run the game
	SplashScreen *splash = new SplashScreen();

	gameApp->setCurrentLevel(splash);
	gameApp->runGame();

	// Cleanup. Deleting the GameApp should delete everything that was part of the game
	delete gameApp;
	return 0;
}