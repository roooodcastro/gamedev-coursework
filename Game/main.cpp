#include <stdlib.h>
#include <iostream>
#include <SDL.h>
#include "GameApp.h"
#include "Model.h"
#include "Texture.h"
#include "ImageItem.h"
#include "ButtonItem.h"

using namespace std;

int main(int argc, char* argv[]) {

	GameApp::initializeContext("GameDev Coursework", 1280, 720);
	GameApp *gameApp = GameApp::getInstance();
	UserInterface *gameInterface = new UserInterface();
	Level *test = new Level(LEVEL_GAME, gameInterface);
	Shader *testShader = new Shader("shaders/testVert.glsl", "shaders/testFrag.glsl", "", "", "");
	Texture *tex = new Texture("perlinnoise.png");
	InterfaceItem *item = new ImageItem(Vector2(10, 10), 0, Vector2(1260, 700));
	ButtonItem *button = new ButtonItem(Vector2(200, 100), 0, Vector2(182, 39), "resources/normal.png", "resources/hovered.png", "resources/pressed.png", "resources/selected.png");
	item->setTexture(tex);
	gameInterface->addItem(item);
	gameInterface->addItem(button);

	gameApp->setDefaultShader(testShader);
	Model *triangle = Model::getTriangle();
	Entity *testTriangle = new Entity();
	testTriangle->setModel(triangle);
	//test->addEntity(testTriangle);
	test->setInterface(gameInterface);
	test->setProjectionMatrix(Matrix4::Orthographic(-1, 1, 1280.0f, 0, 720, 0));
	test->setCameraMatrix(Matrix4::Translation(Vector3(0, 0, 1.0f)));
	gameApp->setCurrentLevel(test);
	gameApp->runGame();

	delete testShader;
	delete triangle;
	delete testTriangle;
	delete test;
	delete gameApp;
	//delete item;
	//delete gameInterface;
	return 0;
}