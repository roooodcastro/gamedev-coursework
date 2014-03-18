#include "GameApp.h"

GameApp *GameApp::instance = NULL;

GameApp::GameApp() {
	gameRunning = false;
	gamePaused = false;
	lastFrameTime = 0;
	lastTickTime = 0;
	lastTickDuration = -1;
	numberOfFrames = 0;
	numberOfTicks = 0;
	frameIntervalSum = 0;
	for (int i = 0; i < 60; i++) {
		frameIntervalList[i] = 0;
	}
	joystick = NULL;
	currentLevel = NULL;
}

GameApp::~GameApp() {
	SDL_DestroyRenderer(renderer);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	if (currentLevel) {
		delete currentLevel;
	}
	delete defaultShader;
}

GameApp *GameApp::initializeContext(const char *gameTitle, const int windowWidth, const int windowHeight) {
	instance = new GameApp();
	// Init all SDL features (Don't care about loading time or memory atm)
	SDL_Init(SDL_INIT_EVERYTHING);
	instance->window = SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (instance->window == nullptr) {
		logSDLError(std::cout, "WINDOW_INIT");
	}
	instance->windowWidth = windowWidth;
	instance->windowHeight = windowHeight;

	// Init and configure OpenGL
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glViewport(0, 0, windowWidth, windowHeight);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	// Enable following line if we get z-fighting problems
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	instance->glContext = SDL_GL_CreateContext(instance->window);
	if (instance->glContext == nullptr) {
		logSDLError(std::cout, "OPENGL_CONTEXT_INIT");
	}
	instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == nullptr) {
		logSDLError(std::cout, "RENDERER_INIT");
	}
	if (TTF_Init() == -1) {
		logSDLError(std::cout, "TTF_INIT");
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLError(std::cout, "IMG_INIT");
	}
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "OGLRenderer::OGLRenderer(): Cannot initialise GLEW!" << std::endl;
	}

	GLenum err_code = glGetError();
	while (GL_NO_ERROR != err_code) {
		printf("OpenGL Error @ CONTEXT_INIT: %i", err_code);
		err_code = glGetError();
	}
	ResourcesManager::initializate();
	// We initialize the primitive meshes that will be used by the interface
	Model::initializePrimitiveMeshes();
	instance->defaultShader = new Shader("shaders/vertNormal.glsl", "shaders/fragLight.glsl", "", "", "");
	//instance->defaultShader = new Shader("shaders/vertUI.glsl", "shaders/fragUI.glsl", "", "", "");

	if (SDL_NumJoysticks() < 1) {
		std::cout << "Warning: No joysticks connected!" << std::endl;
	} else {
		//Load joystick
		instance->joystick = SDL_JoystickOpen(0);
		if (instance->joystick == NULL) {
			printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}

	return instance;
}

void GameApp::runGame() {
	gameRunning = true;
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	startTime = SDL_GetTicks();
	// Start game timers
	installTimers();
	while(gameRunning) {
		handleUserEvents();
		SDL_Delay(1); // Just to not overload the processor. We shouldn't need more than 1000 input checks every second anyway
	}
	// Safely quits SDL
	if (joystick != NULL) {
		SDL_JoystickClose(joystick);
	}
	SDL_Quit();
}

void GameApp::processGameTick(Uint32 millisElapsed) {
	Uint32 tickStart = SDL_GetTicks();
	if (currentLevel) {
		currentLevel->processLevelTick(tickStart - lastTickTime);
	}
	lastTickDuration = SDL_GetTicks() - tickStart;
	lastTickTime = tickStart;
	numberOfTicks++;
}

void GameApp::draw(Uint32 millisElapsed) {
	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Draw current level
	if (currentLevel) {
		currentLevel->drawLevel(millisElapsed);
	}
	// Swap buffers
	SDL_GL_SwapWindow(window);
	// Track fps


	Uint32 frameEnd = SDL_GetTicks();
	Uint32 lastFrameInterval = frameEnd - lastFrameTime;
	frameIntervalSum -= frameIntervalList[numberOfFrames % TARGET_FPS];
	frameIntervalSum += lastFrameInterval;
	frameIntervalList[numberOfFrames % TARGET_FPS] = lastFrameInterval;

	lastFrameTime = frameEnd;
	numberOfFrames++;
}

void GameApp::handleUserEvents() {
	SDL_Event e;
	bool ticked = false;
	bool drawn = false;
	while (SDL_PollEvent(&e)){
		switch (e.type) {
		case SDL_USEREVENT:
			switch (e.user.code) {
			case USER_EVENT_TICK:
				if (!ticked) {
					processGameTick((Uint32) e.user.data1);
					ticked = true;
				}
				break;
			case USER_EVENT_DRAW:
				if (!drawn) {
					draw((Uint32) e.user.data1);
					drawn = true;
				}
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			if (currentLevel) {
				currentLevel->onMouseMoved(Vector2((float) e.motion.x, (float) e.motion.y), Vector2((float) e.motion.xrel, (float) e.motion.yrel));
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (currentLevel) {
				currentLevel->onMouseButtonDown(e.button.button, Vector2((float) e.button.x, (float) e.button.y));
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (currentLevel) {
				currentLevel->onMouseButtonUp(e.button.button, Vector2((float) e.button.x, (float) e.button.y));
				//currentLevel->onMouseClick(e.button.button, Vector2(e.button.x, e.button.y));
				// TODO: Upgrade to SDL 2.0.2 or higher to be able to detec double clicks like this.
				// In the meanwhile, double click events will NOT happen, at all.
				/*if (e.button.clicks >= 2) {
					currentLevel->onMouseDoubleClick(e.button.button, Vector2(e.button.x, e.button.y));
				}*/
			}
			break;
			case SDL_MOUSEWHEEL:
				if (currentLevel) {
					currentLevel->onMouseWheelScroll(e.wheel.y);
				}
				break;
		case SDL_KEYDOWN:
			Keyboard::getInstance()->keyDown(e.key.keysym);
			if (currentLevel) {
				currentLevel->onKeyDown(e.key.keysym);
			}
			break;
		case SDL_KEYUP:
			Keyboard::getInstance()->keyUp(e.key.keysym);
			if (currentLevel) {
				currentLevel->onKeyUp(e.key.keysym);
				currentLevel->onKeyPress(e.key.keysym);
			}
			break;
		case SDL_JOYAXISMOTION:
            if (e.jaxis.which == 0) {
                // X axis motion
                if (e.jaxis.axis == 0) {
					// Fire axis motion event
				}
			}
			break;
		case SDL_JOYBUTTONDOWN:
			// Fire buttond own event
			break;
		case SDL_JOYBUTTONUP:
			// Fire button up event
			break;	
		case SDL_JOYDEVICEADDED:
			// Bind the joystick to the game
			break;
		case SDL_JOYDEVICEREMOVED:
			// Unbind the joystick and warn player
			break;
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_WINDOWEVENT:
			// Checks if user is still controlling the Game Window. If not, then automatically pauses the game.
			// A list of all possible window events can be found here: https://wiki.libsdl.org/SDL_WindowEvent
			switch (e.window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:
				//SDL_Log("Window %d lost keyboard focus", e.window.windowID);
				setGamePaused(true);
				break;
			}
			break;
		}
	}
}

int GameApp::getFps() {
	return (int) (1 / ((frameIntervalSum / TARGET_FPS) / 1000.0f));
}

void GameApp::installTimers() {
    tickTimer = SDL_AddTimer((1000 / TARGET_TPS), ticksLoopTimer, this);
	drawTimer = SDL_AddTimer((1000 / TARGET_FPS), drawLoopTimer, this);
}

Uint32 GameApp::ticksLoopTimer(Uint32 interval, void* param) {
    // Create a user event to call the game loop.
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = USER_EVENT_TICK;
	event.user.data1 = (void*) interval;
    SDL_PushEvent(&event);
    return interval;
}

Uint32 GameApp::drawLoopTimer(Uint32 interval, void* param) {
    // Create a user event to call the game loop.
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = USER_EVENT_DRAW;
    event.user.data1 = (void*) interval;
    SDL_PushEvent(&event);
    return interval;
}

void GameApp::setGamePaused(bool paused) {
	this->gamePaused = paused;
	if (currentLevel) {
		if (paused) {
			currentLevel->onPause();
		} else {
			currentLevel->onResume();
		}
	}
}

void GameApp::setCurrentLevel(Level *level) {
	if (currentLevel) {
		this->currentLevel->onFinish();
	}
	this->currentLevel = level;
	if (currentLevel) {
		this->currentLevel->onStart();
	}
}