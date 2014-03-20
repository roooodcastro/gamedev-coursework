#include <stdlib.h>
#include <iostream>
#include <SDL.h>
#include "GameApp.h"
#include "Model.h"
#include "Texture.h"
#include "ImageItem.h"
#include "ButtonItem.h"
#include "PanelItem.h"
#include "TextItem.h"
#include "Colour.h"
#include "Material.h"
#include "PauseMenu.h"
#include "RaceLevel.h"
#include "Ship.h"
#include "TrackPiece.h"

using namespace std;

int main(int argc, char* argv[]) {

	// Configure game engine for init
	GameApp::initializeContext("GameDev Coursework", 1280, 720);
	GameApp *gameApp = GameApp::getInstance();
	UserInterface gameInterface = UserInterface();
	gameInterface.setShowFpsCounter(true);
	Simulation::getInstance()->setGravity(Simulation::GRAVITY_ZERO);

	// Testing grounds
	// Put test lines below here

	
	




	// Get a panel with some buttons
	/*PanelItem *panel = new PanelItem(Vector2(0, 660), 0, Vector2(1280, 60));
	panel->setTexture(Texture::getOrCreate("0xAA000000", Colour(0xAA000000)));
	ButtonItem *aButton = new ButtonItem(Vector2(100, -2), 0, Vector2(InterfaceItem::SIZE_NO_RESIZE, InterfaceItem::SIZE_NO_RESIZE));
	aButton->setTexture(Texture::getOrCreate("XBOX_A_BUTTON", "resources/xbox/buttons/a.png"));
	ButtonItem *bButton = new ButtonItem(Vector2(400, -2), 0, Vector2(InterfaceItem::SIZE_NO_RESIZE, InterfaceItem::SIZE_NO_RESIZE));
	bButton->setTexture(Texture::getOrCreate("XBOX_B_BUTTON", "resources/xbox/buttons/b.png"));
	TextItem *aText = new TextItem(Vector2(170, 16), 0, "Confirm", 26);
	TextItem *bText = new TextItem(Vector2(470, 16), 0, "Back", 26);
	panel->addInnerItem(aButton);
	panel->addInnerItem(bButton);
	panel->addInnerItem(aText);
	panel->addInnerItem(bText);
	gameInterface.addItem(panel);*/

	// Create a level and run the game
	RaceLevel *testLevel = new RaceLevel();

	gameApp->setCurrentLevel(testLevel);
	gameApp->runGame();

	// Cleanup. Deleting the GameApp should delete everything that was part of the game
	delete gameApp;
	return 0;
}