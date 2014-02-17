#include <stdlib.h>
#include <iostream>
#include <SDL.h>
#include "GameApp.h"
#include "Image.h"

using namespace std;

int main(int argc, char* argv[]) {

	GameApp::initializeContext("GameDev Coursework", 800, 600);
	GameApp *gameApp = GameApp::getInstance();
	UserInterface *gameInterface = new UserInterface();
	Level *test = new Level(LEVEL_GAME, gameInterface);
	gameApp->runGame();
	return 0;
}