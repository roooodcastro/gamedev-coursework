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

using namespace std;

int main(int argc, char* argv[]) {

	GameApp::initializeContext("GameDev Coursework", 1280, 720);
	GameApp *gameApp = GameApp::getInstance();
	UserInterface *gameInterface = new UserInterface();
	Level *test = new Level(LEVEL_GAME, gameInterface);
	Shader *testShader = new Shader("shaders/testVert.glsl", "shaders/testFrag.glsl", "", "", "");
	//Texture *tex = new Texture("perlinnoise.png");

	Texture *font = Texture::createFromText("Hello World!", Colour(255, 255, 255, 255), *TTF_OpenFont("resources/fonts/Arial.ttf", 16));

	InterfaceItem *item = new ImageItem(Vector2(0, 0), 0, Vector2(1280, 720));
	item->setTexture(Texture::createColourTexture(new Colour(0xFFEEEEEE)));
	//item->setTexture(font);
	ButtonItem *button = new ButtonItem(Vector2(200, 100), 0, Vector2(182, 39), "resources/normal.png", "resources/hovered.png", "resources/pressed.png", "resources/selected.png");
	//gameInterface->addItem(item);
	//gameInterface->addItem(button);
	TextItem *text = new TextItem(Vector2(500, 200), 0, "Hello World!", 20);
	//text->setColour(Colour(255, 255, 0, 255));
	PanelItem *panel = new PanelItem(Vector2(0, 300), 0, Vector2(1280, 120));
	panel->setTexture(Texture::createColourTexture(new Colour(0xAA000000)));
	ButtonItem *aButton = new ButtonItem(Vector2(100, 30), 0, Vector2(InterfaceItem::SIZE_NO_RESIZE, InterfaceItem::SIZE_NO_RESIZE));
	aButton->setTexture(new Texture("resources/xbox/buttons/a.png"));
	ButtonItem *bButton = new ButtonItem(Vector2(400, 30), 0, Vector2(InterfaceItem::SIZE_NO_RESIZE, InterfaceItem::SIZE_NO_RESIZE));
	bButton->setTexture(new Texture("resources/xbox/buttons/b.png"));

	panel->addItem(aButton);
	panel->addItem(bButton);
	
	//gameInterface->addItem(panel);
	gameInterface->addItem(text);

	gameInterface->setShowFpsCounter(true);

	gameApp->setDefaultShader(testShader);
	Model *triangle = Model::getTriangle();
	Entity *testTriangle = new Entity();
	testTriangle->setModel(triangle);
	//test->addEntity(testTriangle);
	test->setInterface(gameInterface);
	test->setProjectionMatrix(Matrix4::Orthographic(-1, 1, 1280.0f, 0, 720, 0));
	test->setCameraMatrix(Matrix4::Translation(Vector3(0, 0, 1.0f)));
	gameApp->setCurrentLevel(test);
	gameInterface->setShowFpsCounter(true);
	gameApp->runGame();

	//delete panel;

	delete triangle;
	delete testTriangle;
	delete gameApp;
	//delete item;
	//delete gameInterface;
	return 0;
}