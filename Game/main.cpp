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

using namespace std;

int main(int argc, char* argv[]) {

	// Configure game engine for init
	GameApp::initializeContext("GameDev Coursework", 1280, 720);
	GameApp *gameApp = GameApp::getInstance();
	UserInterface gameInterface = UserInterface();
	Shader *testShader = new Shader("shaders/testVert.glsl", "shaders/testFrag.glsl", "", "", "");
	gameApp->setDefaultShader(testShader);
	gameInterface.setShowFpsCounter(true);

	// Testing grounds
	// Put test lines below here

	// Test material file loading
	std::vector<Material*> materials = Material::loadMaterialsFromFile("resources/models/example.mtl");
	bool hasMaterial = Material::fileHasMaterial("resources/models/example.mtl", "Materialf2");


	// Don't put test lines below here!

	/*
	 * Add some interface items to test them
	 *
	 * Notice the order in which interface items are added is important, as they are just rendered on top of each other,
	 * because it's a 2D plane and we don't (shouldn't!) use the depth buffer.
	 */

	// Make a background colour
	InterfaceItem *background = new ImageItem(Vector2(0, 0), 0, Vector2(1280, 720));
	background->setTexture(Texture::createColourTexture(new Colour(0xFFEEEEEE)));
	gameInterface.addItem(background);

	//item->setTexture(font);
	ButtonItem *button = new ButtonItem(Vector2(200, 100), 0, Vector2(182, 39), "resources/normal.png", "resources/hovered.png", "resources/pressed.png", "resources/selected.png");
	gameInterface.addItem(button);
	TextItem *text = new TextItem(Vector2(500, 200), 0, "Hello World!", 20);
	//text->setColour(Colour(255, 255, 0, 255));

	// Get a panel with some buttons
	PanelItem *panel = new PanelItem(Vector2(0, 660), 0, Vector2(1280, 60));
	panel->setTexture(Texture::createColourTexture(new Colour(0xAA000000)));
	ButtonItem *aButton = new ButtonItem(Vector2(100, -2), 0, Vector2(InterfaceItem::SIZE_NO_RESIZE, InterfaceItem::SIZE_NO_RESIZE));
	aButton->setTexture(new Texture("resources/xbox/buttons/a.png"));
	ButtonItem *bButton = new ButtonItem(Vector2(400, -2), 0, Vector2(InterfaceItem::SIZE_NO_RESIZE, InterfaceItem::SIZE_NO_RESIZE));
	bButton->setTexture(new Texture("resources/xbox/buttons/b.png"));
	TextItem *aText = new TextItem(Vector2(170, 16), 0, "Confirm", 26);
	TextItem *bText = new TextItem(Vector2(470, 16), 0, "Back", 26);
	panel->addItem(aButton);
	panel->addItem(bButton);
	panel->addItem(aText);
	panel->addItem(bText);
	gameInterface.addItem(panel);

	// Create a level and run the game
	Level *testLevel = new Level(LEVEL_GAME, gameInterface);
	testLevel->setProjectionMatrix(Matrix4::Orthographic(-1, 1, 1280.0f, 0, 720, 0));
	testLevel->setCameraMatrix(Matrix4::Translation(Vector3(0, 0, 1.0f)));
	gameApp->setCurrentLevel(testLevel);
	gameApp->runGame();

	// Cleanup. Deleting the GameApp should delete everything that was part of the game
	delete gameApp;
	return 0;
}