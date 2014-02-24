#pragma once

#include "GameApp.h"

enum TextureSlot {
	TEXTURE0,
	TEXTURE1,
	TEXTURE2
};

class Texture {
public:
	Texture(void);
	Texture(char *filename);
	~Texture(void);

	/*
	 * Reads an image file from disk and buffers it to the OpenGL context.
	 * Returns the OpenGL reference value and writes the width and height
	 * of the texture to the two last parameters
	 */
	GLuint loadTexture(char *filename, int &texWidth, int &texHeight);

	/*
	 * Binds an already loaded texture to the specified texture unit.
	 * Call this function right before rendering a model that uses a texture
	 */
	static void bindTexture(Texture *texture, GLuint shaderProgram, TextureSlot slot);

	/*
	 * Binds this texture into the specified texture unit
	 */
	void bindTexture(GLuint shaderProgram, TextureSlot slot);

	GLuint getTextureId() { return textureId; }
	int getTextureWidth() { return texWidth; }
	int getTextureHeight() { return texHeight; }

	/*
	 * Log an SDL error with some error message to the output stream of our choice
	 * @param os The output stream to write the message too
	 * @param msg The error message to write, format will be msg error: SDL_GetError()
	 */
	static void logSDLError(std::ostream &os, const std::string &msg) {
		os << msg << " error: " << SDL_GetError() << std::endl;
	}

protected:

	GLuint textureId;

	/* Texture size parameters */
	int texWidth;
	int texHeight;

};