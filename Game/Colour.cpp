#include "Colour.h"

Colour::Colour(void) {
	this->red = 255;
	this->green = 255;
	this->blue = 255;
	this->alpha = 255;
}

Colour::Colour(const Colour &copy) {
	this->red = copy.red;
	this->green = copy.green;
	this->blue = copy.blue;
	this->alpha = copy.alpha;
}

Colour::Colour(unsigned int colour) {
	this->red = 0;
	this->green = 0;
	this->blue = 0;
	this->alpha = 0;
	this->setColour(colour);
}

Colour::Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;
}

Colour::~Colour(void) {}

Colour &Colour::operator=(const Colour &other) {
	this->red = other.red;
	this->green = other.green;
	this->blue = other.blue;
	this->alpha = other.alpha;
	return *this;
}

void Colour::setColour(uint32_t colour) {
	red = (uint8_t) (colour & 0x000000FF);
	green = (uint8_t) ((colour & 0x0000FF00) >> 8);
	blue = (uint8_t) ((colour & 0x00FF0000) >> 16);
	alpha = (uint8_t) ((colour & 0xFF000000) >> 24);
}

void Colour::setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;
}

unsigned int Colour::getColour() {
	uint32_t colour = 0;
	colour += red;
	colour += (green << 8);
	colour += (blue << 16);
	colour += (alpha << 24);
	return colour;
}