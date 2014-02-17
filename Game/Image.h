#pragma once

#include "GameApp.h"

class Image {
public:
	Image(void);
	~Image(void);


	static SDL_Texture *loadTexture(const char *filename);

	static void renderTexture(SDL_Texture *tex, int x, int y, int w, int h);

	static void renderTexture(SDL_Texture *tex, int x, int y) {
		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
		renderTexture(tex, x, y, w, h);
	}

};