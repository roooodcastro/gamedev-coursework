#include "Texture.h"


Texture::Texture(void) {
	texWidth = -1;
	texHeight = -1;
	textureId = -1;
}

Texture::Texture(const Texture &copy) {
	textureId = copy.textureId;
	texWidth = copy.texWidth;
	texHeight = copy.texHeight;
}

Texture::Texture(char *filename) {
	loadTexture(filename, texWidth, texHeight);
}


Texture::~Texture(void) {
	//glDeleteTextures(1, &textureId);
}

GLuint Texture::loadTexture(char *filename, int &texWidth, int &texHeight) {
	SDL_Surface *surface;
	int mode;
	surface = IMG_Load(filename);
	// Could not load file
	if (!surface) {
		logSDLError(std::cout, "IMAGE_LOAD");
		return 0;
	}
	// work out what format to tell glTexImage2D to use...
	if (surface->format->BytesPerPixel == 3) { // RGB 24bit
		mode = GL_RGB;
	} else if (surface->format->BytesPerPixel == 4) { // RGBA 32bit
		mode = GL_RGBA;
	} else { // Not a valid image
			SDL_FreeSurface(surface);
			return 0;
	}
	this->texWidth = surface->w;
	this->texHeight = surface->h;
	glGenTextures(1, &textureId);

	// this reads from the sdl surface and puts it into an opengl texture
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, texWidth, texHeight, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	// these affect how this texture is drawn later on...
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// clean up
	SDL_FreeSurface(surface);
	GameApp::logOpenGLError("TEX_LOAD");

	return textureId;
}

Texture *Texture::createColourTexture(Uint32 colour) {
	Texture *tex = new Texture();
	tex->texWidth = 1;
	tex->texHeight = 1;
	glGenTextures(1, &(tex->textureId));

	// this reads from the sdl surface and puts it into an opengl texture
	glBindTexture(GL_TEXTURE_2D, tex->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->texWidth, tex->texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colour);

	// these affect how this texture is drawn later on...
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	GameApp::logOpenGLError("TEX_LOAD");

	return tex;
}

Texture *Texture::createColourTexture(Colour *colour) {
	return Texture::createColourTexture(colour->getColour());
}

void Texture::bindTexture(Texture *texture, GLuint shaderProgram, TextureSlot slot) {
	texture->bindTexture(shaderProgram, slot);
}

void Texture::bindTexture(GLuint shaderProgram, TextureSlot slot) {
	GLuint texUnit = GL_TEXTURE0;
	GLuint texVar = -1;
	int texVal = 0;
	switch (slot) {
	case TEXTURE0:
		texVar = glGetUniformLocation(shaderProgram, "texture0");
		break;
	case TEXTURE1:
		texUnit = GL_TEXTURE1;
		texVal = 1;
		texVar = glGetUniformLocation(shaderProgram, "texture1");
		break;
	case TEXTURE2:
		texUnit = GL_TEXTURE2;
		texVal = 2;
		texVar = glGetUniformLocation(shaderProgram, "texture2");
		break;
	}
	if (texVar != -1) {
		glUniform1i(texVar, texVal);
		glActiveTexture(texUnit);
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);
		GameApp::logOpenGLError(((string) "TEX_BIND ") + std::to_string((long long) textureId));
	}
	
}

Texture *Texture::createFromText(std::string textureText, Colour &textColour, TTF_Font &font) {
	Texture *texture = new Texture();
	int mode;
	//TTF_Font *gFont = TTF_OpenFont("resources/fonts/Arial.ttf", 28);
	//Render text surface
	SDL_Color sdlColour = SDL_Color();
	sdlColour.r = textColour.getRed();
	sdlColour.g = textColour.getGreen();
	sdlColour.b = textColour.getBlue();
	sdlColour.a = textColour.getAlpha();
	SDL_Surface* textSurface = TTF_RenderText_Blended(&font, textureText.c_str(), sdlColour);
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	} else {
		// Work out what format to tell glTexImage2D to use...
		if (textSurface->format->BytesPerPixel == 3) { // RGB 24bit
			mode = GL_RGB;
		} else if (textSurface->format->BytesPerPixel == 4) { // RGBA 32bit
			mode = GL_RGBA;
		} else { // Not a valid image
			SDL_FreeSurface(textSurface);
			return 0;
		}
		texture->texWidth = textSurface->w;
		texture->texHeight = textSurface->h;
		glGenTextures(1, &(texture->textureId));

		// this reads from the sdl surface and puts it into an opengl texture
		glBindTexture(GL_TEXTURE_2D, texture->textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, mode, texture->texWidth, texture->texHeight, 0, mode, GL_UNSIGNED_BYTE, textSurface->pixels);

		// these affect how this texture is drawn later on...
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		// clean up
		SDL_FreeSurface(textSurface);
		GameApp::logOpenGLError("TEX_LOAD");
	}
	return texture;
}

Texture &Texture::operator=(const Texture &other) {
	this->textureId = other.textureId;
	this->texWidth = other.texWidth;
	this->texHeight = other.texHeight;
	return *this;
}