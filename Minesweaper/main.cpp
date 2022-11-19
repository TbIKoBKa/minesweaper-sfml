#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <time.h>

#include "components/minesweaper/Minesweaper.h"
#include "misc/constants.h"

using namespace sf;

int main()
{
    srand(time(NULL));

    Font f1;
    f1.loadFromFile("assets/fonts/Nunito-Regular.ttf");

    Minesweaper MinesweaperGame(f1);

    RenderWindow window(VideoMode(MinesweaperGame.windowSize.x, MinesweaperGame.windowSize.y), "Minesweaper");
    window.setKeyRepeatEnabled(false);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event) )
        {
            if (event.type == Event::Closed)
                window.close();

            MinesweaperGame.poolEvent(event, window);
        }

        window.clear();

        MinesweaperGame.drawTo(window);

        window.display();
    }

    return 0;
}