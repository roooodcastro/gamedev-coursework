#include "Image.h"


Image::Image(void)
{
}


Image::~Image(void)
{
}

SDL_Texture *Image::loadTexture(const char *filename) {
	GameApp *context = GameApp::getInstance();
	SDL_Texture *tex = IMG_LoadTexture(context->getRenderer(), filename);
	if (tex == nullptr) {
		return NULL;
	}
	return tex;
}

void Image::renderTexture(SDL_Texture *tex, int x, int y, int w, int h) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	destination.w = w;
	destination.h = h;
	SDL_RenderCopy(GameApp::getInstance()->getRenderer(), tex, NULL, &destination);
}