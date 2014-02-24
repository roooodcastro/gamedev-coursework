#include "GameApp.h"

GameApp *GameApp::instance = NULL;

GameApp::GameApp() {
	gameRunning = false;
	gamePaused = false;
	lastFrameDuration = -1;
	lastTickDuration = -1;
}

GameApp::~GameApp() {
	SDL_DestroyRenderer(renderer);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
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
	// We initialize the primitive meshes that will be used by the interface
	Model::initializePrimitiveMeshes();
	return instance;
}

void GameApp::runGame() {
	gameRunning = true;
	startTime = SDL_GetTicks();
	// Start game timers
	installTimers();
	while(gameRunning) {
		handleUserEvents();
		SDL_Delay(1); // Just to not overload the processor. We shouldn't need more than 1000 input checks every second anyway
	}
	// Safely quits SDL
	SDL_Quit();
}

void GameApp::processGameTick(Uint32 millisElapsed) {
	Uint32 tickStart = SDL_GetTicks();
	if (currentLevel) {
		currentLevel->processLevelTick(millisElapsed);
	}
	lastTickDuration = SDL_GetTicks() - tickStart;
}

void GameApp::draw(Uint32 millisElapsed) {
	Uint32 frameStart = SDL_GetTicks();
	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw current level
	if (currentLevel) {
		currentLevel->drawLevel(millisElapsed);
	}
	// Swap buffers
	SDL_GL_SwapWindow(window);
	lastFrameDuration = SDL_GetTicks() - frameStart;
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
				currentLevel->onMouseMoved(Vector2(e.motion.x, e.motion.y), Vector2(e.motion.xrel, e.motion.yrel));
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (currentLevel) {
				currentLevel->onMouseButtonDown(e.button.button, Vector2(e.button.x, e.button.y));
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (currentLevel) {
				currentLevel->onMouseButtonUp(e.button.button, Vector2(e.button.x, e.button.y));
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
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_WINDOWEVENT:
			// Checks if user is still controlling the Game Window. If not, then automatically pauses the game.
			// A list of all possible window events can be found here: https://wiki.libsdl.org/SDL_WindowEvent
			switch (e.window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:
				//SDL_Log("Window %d lost keyboard focus", e.window.windowID);
				gamePaused = true;
				break;
			}
			break;
		}
	}
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