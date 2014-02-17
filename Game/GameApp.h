#pragma once

#include <map>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Level.h"
#include "Image.h"
#include "Entity.h"
#include "Shader.h"

using namespace std;

class Level;

/*
 * This is the core class of the game engine. This class controls the game loop and holds all main variables and objects necessary to run the game.
 * This is a Singleton class, it must be initialized before calling getInstance(), to start up everything.
 */
class GameApp {

	static GameApp *instance;

public:

	~GameApp(void);

	static GameApp *getInstance() {
		if (instance == nullptr) {
			throw "Game Context must be initialized first!";
		}
		return instance;
	}

	/*
	 * Initializes the SDL and OpenGL context, creating the game window and also reading custom configuration from the config file (TODO if I'm feeling happy)
	 */
	static GameApp *initializeContext(const char *gameTitle, const int windowWidth, const int windowHeight);

	/*
	 * General Getters/Setters
	 */
	SDL_Window *getWindow() { return window; }
	SDL_Renderer *getRenderer() { return renderer; }

	/**
	* Log an SDL error with some error message to the output stream of our choice
	* @param os The output stream to write the message too
	* @param msg The error message to write, format will be msg error: SDL_GetError()
	*/
	static void logSDLError(std::ostream &os, const std::string &msg) {
		os << msg << " error: " << SDL_GetError() << std::endl;
	}

	/*
	 * The actual game loop. Inside this loop the game will check for player input and other minor processing tasks.
	 * This method will only return when the player quits the game or when SDL crashes and shit hits the fan.
	 */
	void runGame();

	/*
	 * Function that will be called to handle special events input, like the player exiting the game, the timer firing the processing methods,
	 * and also some player input like keys and mouse movement. This wil be called in a while(true) loop with 0 or 1ms delay between calls.
	 */
	void handleUserEvents();

	/*
	 * Function that will be called using the timer to process the player inputs, physics, positioning, etc.
	 * The millisElapsed param represents the time since the last call to processGameTick
	 */
	void processGameTick(Uint32 millisElapsed);

	/*
	 * Function that will be called to render the next frame into the back buffer, and flip the buffer afterwards.
	 * The millisElapsed param represents the time since the last call to draw
	 */
	void draw(Uint32 millisElapsed);

	static const int TARGET_FPS = 60; // Frames per second
	static const int TARGET_TPS = 50; // Ticks per second
	static const int USER_EVENT_TICK = 100;
	static const int USER_EVENT_DRAW = 101;
		
private:

	GameApp(void);

	/*
	 * Initializes both tick and draw timers. Those timers are responsible for stabilizing the graphic FPS to around 60,
	 * or as high as possible, and the game ticks (the physics and general processing loop) to around 100 TPS (ticks per second),
	 * creating a smooth running game that is more physically accurate and responds faster to player input while still getting good FPS.
	 */
	void installTimers();

	/*
	 * Callback functions for the timers. Each function basically creates a new event that will be processed by the handleUserEvents
	 * method to either draw a frame or process a game tick.
	 */
	static Uint32 ticksLoopTimer(Uint32 interval, void *param);
	static Uint32 drawLoopTimer(Uint32 interval, void *param);

	int windowWidth; // Window width. Defaults to 1024, but will be read from config file
	int windowHeight; // Window height. Defaults to 768, but will be read from config file
	bool fullscreen; // If the game is running on fullscreen or not. Defaults to false, again, will be read from file
	bool gameRunning; // Self-explanatory, I guess
	bool gamePaused; // Indicated if the game is paused, in which case it should not process any logic
	SDL_Window *window; // The actual desktop window in which SDL and OpenGL will draw to
	SDL_Renderer *renderer; // The SDL renderer, which will be used to interface with OpenGL and draw some sexy pixels
	SDL_GLContext glContext; // The OpenGL Context that will be used to do pretty much everything OpenGL related
	SDL_TimerID tickTimer; // SDL timer to control game ticks
	SDL_TimerID drawTimer; // SDL timer to control FPS
	Uint32 startTime; // When the game starts, this variable will hold the number of millisseconds since SDL initialization
	Uint32 lastFrameDuration; // The amount of time that took to draw the last frame
	Uint32 lastTickDuration; // The amount of time that took to process the last game tick
	map<string, Level> levels; // A map containing the levels loaded for the game
	Level *currentLevel; // A pointer indicating the current Level that is being played
	Shader *defaultShader;
};