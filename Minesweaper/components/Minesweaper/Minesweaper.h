#pragma once

#include <SFML/Graphics.hpp>

#include "../Plate/Plate.h"
#include "../Button/Button.h"
#include "../Textbox/Textbox.h"
#include "../../misc/enums.h"

class Minesweaper
{
public:
    Screen activeScreen;
	bool err = false, isStarted = false, toOpen = false, toMark = false, isClick = false;
	int maxBombs = 10, bombCount = 0, opened = 0, marked = 0, totalMarked = 0, autoOpens = 1;
	sf::Texture hide, bomb, mark, nBombsTexture[9];
	sf::Vector2i poolSize = { 10, 8 };
	sf::Vector2f windowSize = getWindowSize(poolSize);

	Plate** plates = NULL;

    sf::RectangleShape mainMenuContainer;
    sf::RectangleShape infBar;

    sf::Text mainTitle;
    sf::Text subTitle;
    sf::Text showtimer;
    sf::Text result;
    
    Textbox xTextbox;
    Textbox yTextbox;
    
    Button startGameButton;
    Button buttonExit;
    Button buttonReplay;

    sf::Time time;
    sf::Clock clock;

    Minesweaper(sf::Font& font);

	void init();

	void resetGame();

	sf::Vector2f getWindowSize(sf::Vector2i poolSize);

    void poolEvent(sf::Event& event, sf::RenderWindow& window);

    void drawTo(sf::RenderWindow& window);
};

