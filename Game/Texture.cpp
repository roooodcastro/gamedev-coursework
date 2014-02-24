#include "Texture.h"


Texture::Texture(void) {
	textureId = -1;
	texWidth = -1;
	texHeight = -1;
}

Texture::Texture(char *filename) {
	loadTexture(filename, texWidth, texHeight);
}


Texture::~Texture(void) {
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
	texWidth = surface->w;
	texHeight = surface->h;
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

Texture *Texture::createColourTexture(Uint32 *colour) {
	Texture *tex = new Texture();
	tex->texWidth = 1;
	tex->texHeight = 1;
	glGenTextures(1, &(tex->textureId));

	// this reads from the sdl surface and puts it into an opengl texture
	glBindTexture(GL_TEXTURE_2D, tex->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->texWidth, tex->texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, colour);

	// these affect how this texture is drawn later on...
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	GameApp::logOpenGLError("TEX_LOAD");

	return tex;
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
		GameApp::logOpenGLError("TEX_BIND");
	}
	
}