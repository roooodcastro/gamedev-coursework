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

using namespace std;

int main(int argc, char* argv[]) {

	// Configure game engine for init
	GameApp::initializeContext("GameDev Coursework", 1280, 720);
	GameApp *gameApp = GameApp::getInstance();
	UserInterface gameInterface = UserInterface();
	gameInterface.setShowFpsCounter(true);

	// Testing grounds
	// Put test lines below here

	// Test object file loading
	Ship *ship = new Ship(Vector3(0, 0, -20.0f), Vector3(0, 0, 0.0f), Vector3(0, 0, 0));

	Model *trackPieceModel = Model::loadObjFile("resources/models/TrackPiece.obj");
	Entity *trackPiece = new Entity(Vector3(0, 0, -20.0f), Vector3(0, 0, 0), Vector3(0, 90, 0), Vector3(90, 30, 30));
	trackPiece->setModel(trackPieceModel);
	




	// Get a panel with some buttons
	PanelItem *panel = new PanelItem(Vector2(0, 660), 0, Vector2(1280, 60));
	panel->setTexture(Texture::createColourTexture(new Colour(0xAA000000)));
	ButtonItem *aButton = new ButtonItem(Vector2(100, -2), 0, Vector2(InterfaceItem::SIZE_NO_RESIZE, InterfaceItem::SIZE_NO_RESIZE));
	aButton->setTexture(new Texture("resources/xbox/buttons/a.png"));
	ButtonItem *bButton = new ButtonItem(Vector2(400, -2), 0, Vector2(InterfaceItem::SIZE_NO_RESIZE, InterfaceItem::SIZE_NO_RESIZE));
	bButton->setTexture(new Texture("resources/xbox/buttons/b.png"));
	TextItem *aText = new TextItem(Vector2(170, 16), 0, "Confirm", 26);
	TextItem *bText = new TextItem(Vector2(470, 16), 0, "Back", 26);
	panel->addInnerItem(aButton);
	panel->addInnerItem(bButton);
	panel->addInnerItem(aText);
	panel->addInnerItem(bText);
	gameInterface.addItem(panel);

	// Create a level and run the game
	RaceLevel *testLevel = new RaceLevel();
	testLevel->setCameraMatrix(Matrix4::Translation(Vector3(0, 0, -10.0f)));
	testLevel->addEntity(ship);
	testLevel->addEntity(trackPiece);

	gameApp->setCurrentLevel(testLevel);
	gameApp->runGame();

	// Cleanup. Deleting the GameApp should delete everything that was part of the game
	delete gameApp;
	return 0;
}