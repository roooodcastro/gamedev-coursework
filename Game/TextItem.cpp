#include "TextItem.h"

const string TextItem::defaultFont = "resources/fonts/Arial.ttf";
const int TextItem::defaultFontSize = 16;
const Colour TextItem::defaultColour = Colour(0xFFFFFFFF);

TextItem::TextItem(void) : InterfaceItem(ITEM_TEXT) {
	font = defaultFont;
	fontSize = defaultFontSize;
	colour = new Colour(defaultColour);
	setText("");
}

TextItem::TextItem(const TextItem &copy) : InterfaceItem(copy) {
	this->text = string(copy.text);
	this->font = string(copy.font);
	this->fontSize = copy.fontSize;
	this->colour = new Colour(*(copy.colour));
}

TextItem::TextItem(string text, int fontSize) : InterfaceItem(ITEM_TEXT) {
	font = defaultFont;
	fontSize = fontSize;
	colour = new Colour(defaultColour);
	setText(text);
}

TextItem::TextItem(Vector2 &position, float rotation, string text, int fontSize) : InterfaceItem(ITEM_TEXT, position, rotation, Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE)) {
	font = defaultFont;
	this->fontSize = fontSize;
	colour = new Colour(defaultColour);
	setText(text);
	this->size = new Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE);
}

TextItem::TextItem(Vector2 &position, float rotation, Vector2 &size) : InterfaceItem(ITEM_TEXT, position, rotation, size) {
	font = defaultFont;
	fontSize = defaultFontSize;
	colour = new Colour(defaultColour);
	setText("");
}

TextItem::TextItem(Vector2 &position, float rotation, Vector2 &size, string text, int fontSize) : InterfaceItem(ITEM_TEXT, position, rotation, size) {
	font = defaultFont;
	this->fontSize = fontSize;
	colour = new Colour(defaultColour);
	setText(text);
}

TextItem::~TextItem(void) {
	delete colour;
}

void TextItem::setText(string text) {
	this->text = text;
	reloadText();
}

void TextItem::setColour(Colour &colour) {
	this->colour = &colour;
	reloadText();
}

void TextItem::setFont(string font) {
	this->font = font;
	reloadText();
}

void TextItem::setFontSize(int fontSize) {
	this->fontSize = fontSize;
	reloadText();
}

void TextItem::reloadText() {
	TTF_Font *ttfFont = TTF_OpenFont(font.c_str(), fontSize);
	// We release our current texture before creating the new one
	if (texture) {
		GLuint texId = texture->getTextureId();
		glDeleteTextures(1, &texId);
		delete texture;
	}
	texture = Texture::createFromText(text, *colour, *ttfFont);
	TTF_CloseFont(ttfFont);
}

void TextItem::onMouseMoved(Vector2 &position, Vector2 &amount) {
}

void TextItem::onMouseClick(Uint8 button, Vector2 &position) {
}

void TextItem::onMouseDoubleClick(Uint8 button, Vector2 &position) {
}

void TextItem::onMouseButtonDown(Uint8 button, Vector2 &position) {
}

void TextItem::onMouseButtonUp(Uint8 button, Vector2 &position) {
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void TextItem::onMouseWheelScroll(int amount) {
}

void TextItem::onKeyPress(SDL_Keysym key) {
}

void TextItem::onKeyDown(SDL_Keysym key) {
}

void TextItem::onKeyUp(SDL_Keysym key) {
}

TextItem &TextItem::operator=(const TextItem &other) {
	this->text = other.text;
	this->font = other.font;
	this->fontSize = other.fontSize;
	this->colour = other.colour;
	return *this;
}