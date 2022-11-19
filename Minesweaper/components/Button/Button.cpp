#include "Button.h"

Button::Button() {
	btnHeight = 0;
	btnWidth = 0;
}

Button::Button(
	std::string textButton,
	sf::Vector2f buttonSize,
	int fontSize,
	sf::Color backgroundColor,
	sf::Color textColor,
	sf::Font& font
) {
	button.setSize(buttonSize);
	button.setFillColor(backgroundColor);

	// Get these for later use:
	btnWidth = buttonSize.x;
	btnHeight = buttonSize.y;

	text.setString(textButton);
	text.setCharacterSize(fontSize);
	text.setFillColor(textColor);
	text.setFont(font);
}

void Button::setFont(sf::Font& font) {
	text.setFont(font);
}

void Button::setBackColor(sf::Color color) {
	button.setFillColor(color);
}

void Button::setTextColor(sf::Color color) {
	text.setFillColor(color);
}

void Button::setPosition(sf::Vector2f point) {
	button.setPosition(point);

	// Center text on button:
	float xPos = (point.x + (float)btnWidth / 2) - (text.getLocalBounds().width / 2);
	float yPos = (point.y + (float)btnHeight / 2) - (text.getLocalBounds().height / 2 + 5);
	text.setPosition(xPos, yPos);
}

void Button::drawTo(sf::RenderWindow& window) {
	window.draw(button);
	window.draw(text);
}

bool Button::isMouseOver(sf::RenderWindow& window) {
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f pos = window.mapPixelToCoords(pixelPos);

	if (button.getGlobalBounds().contains(pos.x, pos.y)) {
		return true;
	}
	return false;
}
