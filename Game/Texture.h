#pragma once

#include "GameApp.h"
#include "Colour.h"

enum TextureSlot {
	TEXTURE0,
	TEXTURE1,
	TEXTURE2
};

class Texture {
public:
	Texture(void);
	Texture(const Texture &copy);
	Texture(char *filename);
	~Texture(void);

	/*
	 * Reads an image file from disk and buffers it to the OpenGL context.
	 * Returns the OpenGL reference value and writes the width and height
	 * of the texture to the two last parameters
	 */
	GLuint loadTexture(char *filename, int &texWidth, int &texHeight);

	/*
	 * Creates an OpenGL texture from a colour value.
	 * This texture will be only 1x1px in size, and should be stretched along
	 * the whole shape.
	 */
	static Texture *createColourTexture(Uint32 colour);
	static Texture *createColourTexture(Colour *colour);

	static Texture *createFromText(std::string textureText, Colour &textColour, TTF_Font &font);

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
	bool isTextureValid() { return textureId >= 0 && textureId < 4000000; }

	/*
	 * Log an SDL error with some error message to the output stream of our choice
	 * @param os The output stream to write the message too
	 * @param msg The error message to write, format will be msg error: SDL_GetError()
	 */
	static void logSDLError(std::ostream &os, const std::string &msg) {
		os << msg << " error: " << SDL_GetError() << std::endl;
	}

	Texture &operator=(const Texture &other);

protected:

	GLuint textureId;

	/* Texture size parameters */
	int texWidth;
	int texHeight;

};