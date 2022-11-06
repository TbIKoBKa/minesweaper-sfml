#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include "plate.h"

#define GEN_1

using namespace sf;

int main()
{
    srand(time(NULL));
    float SCALE = 1.0;
    bool isClick = false;
    bool err = false, isStarted = false, isEnd = false, toOpen = false, toMark = false;
    int maxBombs = 10, bombCount = 0, opened = 0, marked = 0, totalMarked = 0, autoOpens = 1;
    int poolSize[2] = { 10, 8 };
    std::string bCode[9] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight" };
    Plate** plates;

    RenderWindow window(VideoMode(32*poolSize[0], 32*poolSize[1]+50), "Minesweaper");
    window.setKeyRepeatEnabled(false);

    RectangleShape infBar(sf::Vector2f(32 * poolSize[0], 50));
    infBar.setFillColor(Color::Cyan);
    infBar.setPosition(sf::Vector2f(0, 32 * poolSize[1]));
    
    RectangleShape buttonExit(sf::Vector2f(100, 50));
    buttonExit.setFillColor(Color::Green);
    buttonExit.setPosition(sf::Vector2f(32*poolSize[0]/2-50, 32 * poolSize[1]-20));
    
    Font f1;
    f1.loadFromFile("arial.ttf");

    Text debug;
    debug.setFont(f1);
    debug.setCharacterSize(24);
    debug.setFillColor(Color::White);

    Text showtimer;
    showtimer.setFont(f1);
    showtimer.setCharacterSize(24);
    showtimer.setFillColor(Color::Black);
    showtimer.setPosition(10, 32*poolSize[1]+10);
    showtimer.setString("0");

    Text result;
    result.setFont(f1);
    result.setCharacterSize(50);
    result.setFillColor(Color::Black);
    result.setPosition(30, 32 * poolSize[1]/ 2 -30);

    Text captureExitButton;
    captureExitButton.setFont(f1);
    captureExitButton.setCharacterSize(24);
    captureExitButton.setFillColor(Color::Black);
    captureExitButton.setPosition(buttonExit.getPosition().x+30, buttonExit.getPosition().y+10);
    captureExitButton.setString("Exit");

    Texture hide;
    Texture bomb;
    Texture mark;
    Texture nBombsTexture[9];

    Time time;
    Clock clock;

    if (!hide.loadFromFile("textures/plate_hide.png"))
    {
        printf("ERROR:Texture hide\n");
        err = true;
    }
    if (!bomb.loadFromFile("textures/plate_bomb.png"))
    {
        printf("ERROR:Texture bomb\n");
        err = true;
    }
    if (!mark.loadFromFile("textures/plate_mark.png"))
    {
        printf("ERROR:Texture mark\n");
        err = true;
    }
    for (int i = 0; i < 9; i++)
    {
        if (!nBombsTexture[i].loadFromFile("textures/plate_" + bCode[i] + ".png"))
        {
            printf("ERROR:Texture nBombs %s\n", bCode[i].c_str());
            err = true;
        }
    }

    plates = new Plate * [poolSize[0]];

    for (int i = 0; i < poolSize[0]; i++)
    {
        plates[i] = new Plate[poolSize[1]];
    }

    for (int i = 0; i < poolSize[0]; i++)
    {
        for (int j = 0; j < poolSize[1]; j++)
        {
            plates[i][j].sprite.setTexture(hide);
            plates[i][j].sprite.setPosition(i * 32, j * 32);
            plates[i][j].isBomb = false;
            plates[i][j].isClosed = true;
            plates[i][j].isMarked = false;
            plates[i][j].nBombs = 0;
        }
    }

    Vector2i pixelPos = Mouse::getPosition(window);
    Vector2f pos = window.mapPixelToCoords(pixelPos);

#ifdef GEN_2
    while (bombCount != maxBombs)
    {
        for (int i = 0; i < poolSize[0]; i++)
        {
            for (int j = 0; j < poolSize[1]; j++)
            {
                if (bombCount >= maxBombs)
                {
                    break;
                }
                else if (plates[i][j].isBomb == true)
                {
                    continue;
                }
                else
                {
                    gen = rand() % 100;
                    std::cout << gen << std::endl;
                    if (gen > 0 && gen <= 20)
                    {
                        plates[i][j].isBomb = true;
                        plates[i][j].sprite.setColor(Color::Red);
                        bombCount++;
                    }
                }
            }
        }
    }
#endif
#ifdef GEN_1
    int b_per_line = (float)maxBombs / (float)poolSize[0];

    int n = 0, lc = 0, bombscount = 0;
    while (bombscount < maxBombs)
    {
        for (int i = 0; i < poolSize[0]; i++)
        {
            while (lc < b_per_line)
            {
                if (bombscount >= maxBombs)
                    break;
                n = rand() % poolSize[1];
                if (!plates[i][n].isBomb)
                {
                    plates[i][n].isBomb = true;
                    //
                    //plates[i][n].sprite.setColor(Color::Red);
                    //
                    lc++;
                    bombscount++;
                }
            }
            lc = 0;
        }
    }
#endif

    for (int i = 0; i < poolSize[0]; i++)
    {
        for (int j = 0; j < poolSize[1]; j++)
        {
            if (!plates[i][j].isBomb)
            {
                for (int iu = i - 1, id = i + 1, jl = j - 1, jr = j + 1; jl < jr+1; jl++)
                {
                    if (jl >= 0 && jl <= poolSize[1] - 1)
                    {
                        if (iu >= 0 && plates[iu][jl].isBomb)
                            plates[i][j].nBombs++;
                        if (plates[i][jl].isBomb)
                            plates[i][j].nBombs++;
                        if (id < poolSize[0] && plates[id][jl].isBomb)
                            plates[i][j].nBombs++;
                    }
                }
            }
        }
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event) )
        {
            if (!isEnd)
            {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::MouseMoved)
                {
                    pixelPos = Mouse::getPosition(window);
                    pos = window.mapPixelToCoords(pixelPos);
                }

                if (event.type == Event::MouseButtonPressed)
                {
                    isClick = true;

                    switch (event.mouseButton.button)
                    {
                    case Mouse::Left:
                        toOpen = true;
                        break;
                    case Mouse::Right:
                        toMark = true;
                        break;
                    default:
                        break;
                    }

                    for (int i = 0; i < poolSize[0]; i++)
                    {
                        for (int j = 0; j < poolSize[1]; j++)
                        {
                            if (plates[i][j].sprite.getGlobalBounds().contains(pos.x, pos.y))
                            {
                                if (!isStarted)
                                    isStarted = true;
                                if (!plates[i][j].isBomb)
                                {
                                    std::cout << plates[i][j].nBombs;
                                }
                                if (plates[i][j].isClosed)
                                {
                                    std::cout << "not closed" << std::endl;
                                    if (toOpen == true && !plates[i][j].isMarked)
                                    {
                                        if (plates[i][j].isBomb)
                                        {
                                            isEnd = true;
                                            result.setString("You loose :(");
                                            //window.close();
                                        }
                                        plates[i][j].sprite.setTexture(nBombsTexture[plates[i][j].nBombs]);
                                        plates[i][j].isClosed = false;
                                        opened++;

                                        if (plates[i][j].nBombs == 0)
                                        {
                                            while (autoOpens > 0)
                                            {
                                                autoOpens = 0;
                                                for (int i = 0; i < poolSize[0]; i++)
                                                {
                                                    for (int j = 0; j < poolSize[1]; j++)
                                                    {
                                                        if (!plates[i][j].isClosed && plates[i][j].nBombs == 0)
                                                        {
                                                            for (int iu = i - 1, id = i + 1, jl = j - 1, jr = j + 1; jl < jr + 1; jl++)
                                                            {
                                                                if (jl >= 0 && jl <= poolSize[1] - 1)
                                                                {
                                                                    if (iu >= 0 && !plates[iu][jl].isBomb && plates[iu][jl].isClosed)
                                                                    {
                                                                        plates[iu][jl].isClosed = false;
                                                                        opened++;
                                                                        plates[iu][jl].sprite.setTexture(nBombsTexture[plates[iu][jl].nBombs]);
                                                                        autoOpens++;
                                                                    }
                                                                    if (!plates[i][jl].isBomb && j != jl && plates[i][jl].isClosed)
                                                                    {
                                                                        plates[i][jl].isClosed = false;
                                                                        opened++;
                                                                        plates[i][jl].sprite.setTexture(nBombsTexture[plates[i][jl].nBombs]);
                                                                        autoOpens++;
                                                                    }
                                                                    if (id < poolSize[0] && !plates[id][jl].isBomb && plates[id][jl].isClosed)
                                                                    {
                                                                        plates[id][jl].isClosed = false;
                                                                        opened++;
                                                                        plates[id][jl].sprite.setTexture(nBombsTexture[plates[id][jl].nBombs]);
                                                                        autoOpens;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            autoOpens = 1;
                                        }
                                    }
                                    else if (toMark == true)
                                    {
                                        if (!plates[i][j].isMarked)
                                        {
                                            plates[i][j].sprite.setTexture(mark);
                                            plates[i][j].isMarked = true;
                                            totalMarked++;
                                            if (plates[i][j].isBomb)
                                                marked++;
                                        }
                                        else if (plates[i][j].isMarked)
                                        {
                                            plates[i][j].sprite.setTexture(hide);
                                            plates[i][j].isMarked = false;
                                            totalMarked--;
                                            if (plates[i][j].isBomb)
                                                marked--;
                                        }
                                    }
                                }
                                toOpen = false;
                                toMark = false;
                            }
                        }
                        //std::cout << std::endl << opened << " opened" << std::endl;
                    }
                }
                if (event.type == Event::MouseButtonReleased)
                    isClick = false;

                if ((marked == maxBombs && totalMarked == marked) || opened == poolSize[0] * poolSize[1] - maxBombs)
                {
                    result.setString("You win :)");
                    isEnd = true;
                    //window.close();
                }
            }
            else if (isEnd)
            {
                if (event.type == Event::MouseMoved)
                {
                    pixelPos = Mouse::getPosition(window);
                    pos = window.mapPixelToCoords(pixelPos);
                }

                if (event.type == Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        if (buttonExit.getGlobalBounds().contains(pos.x, pos.y))
                        {
                            window.close();
                        }
                    }
                }
            }
        }

        if (isStarted && !isEnd)
        {
            time = clock.getElapsedTime();
            showtimer.setString(std::to_string(int(time.asSeconds())));
        }
        else
        {
            clock.restart();
        }

        window.clear();
        if (!isEnd)
        {
            for (int i = 0; i < poolSize[0]; i++)
            {
                for (int j = 0; j < poolSize[1]; j++)
                {
                    window.draw(plates[i][j].sprite);
                }
            }
            window.draw(infBar);
            window.draw(showtimer);
        }
        else if(isEnd)
        {
            infBar.setFillColor(Color::Yellow);
            infBar.setSize(Vector2f(32 * poolSize[0], 32 * poolSize[1] + 50));
            infBar.setPosition(0, 0);
            window.draw(infBar);
            window.draw(result);
            window.draw(showtimer);
            window.draw(buttonExit);
            window.draw(captureExitButton);
        }

        window.display();
    }

    return 0;
}