#include <stdlib.h>
#include <iostream>
#include <SDL.h>
#include "GameApp.h"
#include "Model.h"

using namespace std;

int main(int argc, char* argv[]) {

	GameApp::initializeContext("GameDev Coursework", 800, 600);
	GameApp *gameApp = GameApp::getInstance();
	UserInterface *gameInterface = new UserInterface();
	Level *test = new Level(LEVEL_GAME, gameInterface);
	Shader *testShader = new Shader("testVert.glsl", "testFrag.glsl", "", "", "");
	gameApp->setDefaultShader(testShader);
	Model *triangle = Model::generateTriangle();
	Entity *testTriangle = new Entity();
	testTriangle->setModel(triangle);
	test->addEntity(testTriangle);
	test->setProjectionMatrix(Matrix4::Orthographic(-1, 1, 1, -1, 1, -1));
	test->setCameraMatrix(Matrix4::Translation(Vector3(0, 0, -1.0f)));
	gameApp->setCurrentLevel(test);
	gameApp->runGame();

	delete testShader;
	delete triangle;
	delete testTriangle;
	delete test;
	delete gameApp;
	return 0;
}