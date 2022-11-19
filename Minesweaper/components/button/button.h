#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Button {
public:
	Button();

	Button(
		std::string text,
		sf::Vector2f buttonSize,
		int fontSize,
		sf::Color backgroundColor,
		sf::Color textColor,
		sf::Font& font
	);

	void setFont(sf::Font& fonts);

	void setBackColor(sf::Color color);

	void setTextColor(sf::Color color);

	void setPosition(sf::Vector2f point);

	void drawTo(sf::RenderWindow& window);

	bool isMouseOver(sf::RenderWindow& window);
private:
	sf::RectangleShape button;
	sf::Text text;

	int btnWidth;
	int btnHeight;
};