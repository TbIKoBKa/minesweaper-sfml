#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

// Define keys:
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class Textbox {
public:
	Textbox();

	Textbox(
		sf::Vector2f boxSize, 
		int fontSize, 
		sf::Color backgroundColor,
		sf::Color outlineColor, 
		sf::Color textColor,
		int outlineSize,
		sf::Font& font, 
		bool defaultSelected
	);

	void setFont(sf::Font& fonts);

	void setPosition(sf::Vector2f point);

	// Set char limits:
	void setLimit(bool ToF);

	void setLimit(bool ToF, int lim);

	// Change selected state:
	void setSelected(bool sel);

	std::string getText();

	void drawTo(sf::RenderWindow& window);

	// Function for event loop:
	void typedOn(sf::Event input);

	bool isMouseOver(sf::RenderWindow& window);

	bool getSelected();
private:
	sf::RectangleShape box;
	sf::Text textbox;

	std::ostringstream text;
	bool isSelected = false;
	bool hasLimit = false;
	int limit = 0;
	int width;
	int height;

	// Delete the last character of the text:
	void deleteLastChar();

	// Get user input:
	void inputLogic(int charTyped);
};