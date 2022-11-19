#pragma once

#include <SFML/Graphics.hpp>

class Plate {
public:
	sf::Sprite sprite;
	sf::Texture def;
	bool isClosed;
	bool isMarked;
	bool isBomb;
	int nBombs;

	~Plate()
	{
		;
	}
};