#include "Colour.h"

Colour::Colour(void) {
	this->red = new uint8_t();
	this->green = new uint8_t();
	this->blue = new uint8_t();
	this->alpha = new uint8_t();
}

Colour::Colour(unsigned int colour) {
	this->red = new uint8_t();
	this->green = new uint8_t();
	this->blue = new uint8_t();
	this->alpha = new uint8_t();
	this->setColour(colour);
}

Colour::Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	this->red = new uint8_t(red);
	this->green = new uint8_t(green);
	this->blue = new uint8_t(blue);
	this->alpha = new uint8_t(alpha);
}

void Colour::setColour(uint32_t colour) {
	*red = (uint8_t) (colour & 0x000000FF);
	*green = (uint8_t) ((colour & 0x0000FF00) >> 8);
	*blue = (uint8_t) ((colour & 0x00FF0000) >> 16);
	*alpha = (uint8_t) ((colour & 0xFF000000) >> 24);
}

void Colour::setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	*(this->red) = red;
	*(this->green) = green;
	*(this->blue) = blue;
	*(this->alpha) = alpha;
}

unsigned int Colour::getColour() {
	uint32_t colour = 0;
	colour += (*red);
	colour += (*green << 8);
	colour += (*blue << 16);
	colour += (*alpha << 24);
	return colour;
}

Colour::~Colour(void) {
	delete red;
	delete green;
	delete blue;
	delete alpha;
}
