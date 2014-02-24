/*
 * Author: Rodrigo Castro Azevedo
 * 
 * Description: A helper class to represent a RGBA colour.
 * This class should be primarily be used with OpenGL,
 * the Uint32 outputs of the methods in this class are
 * in the OpenGL format, which is basically the regular
 * RGBA format, but in reverse endianess (ABGR).
 * A colour that may be represented in hexadecimal as
 * 0xRRGGBBAA will be represented as 0xAABBGGRR in this class.
 */

#pragma once

#include <stdint.h>

class Colour {
public:

	Colour(void);
	Colour(uint32_t colour);
	Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	~Colour(void);

	void setColour(uint32_t colour);
	void setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	void setRed(uint8_t red) { *(this->red) = red; }
	void setGreen(uint8_t green) { *(this->green) = green; }
	void setBlue(uint8_t blue) { *(this->blue) = blue; }
	void setAlpha(uint8_t alpha) { *(this->alpha) = alpha; }
	uint32_t getColour();
	uint8_t getRed() { return *red; }
	uint8_t getGreen() { return *green; }
	uint8_t getBlue() { return *blue; }
	uint8_t getAlpha() { return *alpha; }

protected:

	uint8_t *red;
	uint8_t *green;
	uint8_t *blue;
	uint8_t *alpha;
};

