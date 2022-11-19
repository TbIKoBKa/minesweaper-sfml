#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <random>
#include <time.h>

#include "components/button/button.h"
#include "components/textbox/textbox.h"
#include "components/plate/plate.h"
#include "misc/enums.h"

#define GEN_1

using namespace sf;

Vector2f getWindowSize(Vector2i poolSize) {
    return Vector2f(32 * poolSize.x, 32 * poolSize.y + 50);
}

void generatePool(Plate **&plates, Vector2i &size, int maxBombs, Texture &hideTexture) {
    plates = new Plate * [size.x];

    for (int i = 0; i < size.x; i++)
    {
        plates[i] = new Plate[size.y];

        for (int j = 0; j < size.y; j++)
        {
            plates[i][j].sprite.setTexture(hideTexture);
            plates[i][j].sprite.setPosition(i * 32, j * 32);
            plates[i][j].isBomb = false;
            plates[i][j].isClosed = true;
            plates[i][j].isMarked = false;
            plates[i][j].nBombs = 0;
        }
    }

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
    int b_per_line = (float)maxBombs / (float)size.x;

    int n = 0, lc = 0, bombscount = 0;
    while (bombscount < maxBombs)
    {
        for (int i = 0; i < size.x; i++)
        {
            while (lc < b_per_line)
            {
                if (bombscount >= maxBombs)
                    break;
                n = rand() % size.y;
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

    for (int i = 0; i < size.x; i++)
    {
        for (int j = 0; j < size.y; j++)
        {
            if (!plates[i][j].isBomb)
            {
                for (int iu = i - 1, id = i + 1, jl = j - 1, jr = j + 1; jl < jr + 1; jl++)
                {
                    if (jl >= 0 && jl <= size.y - 1)
                    {
                        if (iu >= 0 && plates[iu][jl].isBomb)
                            plates[i][j].nBombs++;
                        if (plates[i][jl].isBomb)
                            plates[i][j].nBombs++;
                        if (id < size.x && plates[id][jl].isBomb)
                            plates[i][j].nBombs++;
                    }
                }
            }
        }
    }
}

int main()
{
    srand(time(NULL));
    float SCALE = 1.0;
    Screen screen = MAIN_MENU_SCREEN;
    bool isClick = false;
    bool err = false, isStarted = false, toOpen = false, toMark = false;
    int maxBombs = 10, bombCount = 0, opened = 0, marked = 0, totalMarked = 0, autoOpens = 1;
    Vector2i poolSize = { 10, 8 };
    std::string bCode[9] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight" };
    Plate** plates = NULL;

    Vector2f windowSize = getWindowSize(poolSize);
    RenderWindow window(VideoMode(windowSize.x, windowSize.y), "Minesweaper");
    window.setKeyRepeatEnabled(false);

    Font f1;
    f1.loadFromFile("assets/fonts/Nunito-Regular.ttf");

    RectangleShape mainMenuContainer(windowSize);
    mainMenuContainer.setFillColor(Color::White);
    Text mainTitle("Minesweaper", f1, 28);
    mainTitle.setPosition(Vector2f(windowSize.x / 2 - mainTitle.getLocalBounds().width / 2, 10));
    mainTitle.setFillColor(Color::Black);
    Textbox xTextbox(Vector2f(windowSize.x - 20, 30), 18, Color::Black, Color::White, f1, false);
    Textbox yTextbox(Vector2f(windowSize.x - 20, 30), 18, Color::Black, Color::White, f1, false);
    xTextbox.setPosition(Vector2f(10, 40));
    yTextbox.setPosition(Vector2f(10, 80));
    Button startGameButton("Start", { 90, 40 }, 18, Color::Black, Color::White, f1);
    startGameButton.setPosition(Vector2f(windowSize.x / 2 - 45, windowSize.y - 40 - 10));

    RectangleShape infBar(Vector2f(windowSize.x, 50));
    infBar.setFillColor(Color::Cyan);
    infBar.setPosition(Vector2f(0, windowSize.y - 50));
    
    Button buttonExit("Exit", { 90, 40 }, 18, Color::Red, Color::White, f1);
    buttonExit.setPosition(Vector2f(32 * poolSize.x / 2 - 50, 32 * poolSize.y - 20));
    
    Button buttonReplay("Replay", { 90, 40 }, 18, Color::Green, Color::White, f1);
    buttonReplay.setPosition(Vector2f(32 * poolSize.x / 2 - 50, 32 * poolSize.y - 70));

    Text debug;
    debug.setFont(f1);
    debug.setCharacterSize(24);
    debug.setFillColor(Color::White);

    Text showtimer;
    showtimer.setFont(f1);
    showtimer.setCharacterSize(24);
    showtimer.setFillColor(Color::Black);
    showtimer.setPosition(10, 32*poolSize.y+10);
    showtimer.setString("0");

    Text result;
    result.setFont(f1);
    result.setCharacterSize(50);
    result.setFillColor(Color::Black);
    result.setPosition(30, 32 * poolSize.y/ 2 -30);

    Texture hide, bomb, mark, nBombsTexture[9];

    Time time;
    Clock clock;

    if (!hide.loadFromFile("assets/textures/plate_hide.png"))
    {
        printf("ERROR:Texture hide\n");
        err = true;
    }
    if (!bomb.loadFromFile("assets/textures/plate_bomb.png"))
    {
        printf("ERROR:Texture bomb\n");
        err = true;
    }
    if (!mark.loadFromFile("assets/textures/plate_mark.png"))
    {
        printf("ERROR:Texture mark\n");
        err = true;
    }
    for (int i = 0; i < 9; i++)
    {
        if (!nBombsTexture[i].loadFromFile("assets/textures/plate_" + bCode[i] + ".png"))
        {
            printf("ERROR:Texture nBombs %s\n", bCode[i].c_str());
            err = true;
        }
    }

    Vector2i pixelPos = Mouse::getPosition(window);
    Vector2f pos = window.mapPixelToCoords(pixelPos);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event) )
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::Resized)
                windowSize = Vector2f(event.size.width, event.size.height);

            switch (screen) {
            case MAIN_MENU_SCREEN:
                if (event.type == Event::MouseButtonPressed) {
                    if (xTextbox.isMouseOver(window)) {
                        xTextbox.setSelected(true);
                        yTextbox.setSelected(false);
                    }
                    else if (yTextbox.isMouseOver(window)) {
                        yTextbox.setSelected(true);
                        xTextbox.setSelected(false);
                    }
                    else if (startGameButton.isMouseOver(window)) {
                        screen = IN_GAME_SCREEN;
                        //poolSize = Vector2i(atoi(xTextbox.getText().c_str()), atoi(yTextbox.getText().c_str()));
                        //windowSize = getWindowSize(poolSize);
                        generatePool(plates, poolSize, maxBombs, hide);
                    }
                    else {
                        xTextbox.setSelected(false);
                        yTextbox.setSelected(false);
                    }
                }
                else if (event.type == Event::TextEntered) {
                    if (xTextbox.getSelected()) {
                        xTextbox.typedOn(event);
                    }
                    else if (yTextbox.getSelected()) {
                        yTextbox.typedOn(event);
                    }
                }
                break;
            case IN_GAME_SCREEN:
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

                    for (int i = 0; i < poolSize.x; i++)
                    {
                        for (int j = 0; j < poolSize.y; j++)
                        {
                            if (plates[i][j].sprite.getGlobalBounds().contains(pos.x, pos.y))
                            {
                                if (!isStarted)
                                    isStarted = true;

                                if (plates[i][j].isClosed)
                                {
                                    if (toOpen && !plates[i][j].isMarked)
                                    {
                                        if (plates[i][j].isBomb)
                                        {
                                            screen = END_SCREEN;
                                            result.setString("You loose :(");
                                        }
                                        plates[i][j].sprite.setTexture(nBombsTexture[plates[i][j].nBombs]);
                                        plates[i][j].isClosed = false;
                                        opened++;

                                        if (plates[i][j].nBombs == 0)
                                        {
                                            while (autoOpens > 0)
                                            {
                                                autoOpens = 0;
                                                for (int i = 0; i < poolSize.x; i++)
                                                {
                                                    for (int j = 0; j < poolSize.y; j++)
                                                    {
                                                        if (!plates[i][j].isClosed && plates[i][j].nBombs == 0)
                                                        {
                                                            for (int iu = i - 1, id = i + 1, jl = j - 1, jr = j + 1; jl < jr + 1; jl++)
                                                            {
                                                                if (jl >= 0 && jl <= poolSize.y - 1)
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
                                                                    if (id < poolSize.x && !plates[id][jl].isBomb && plates[id][jl].isClosed)
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
                                    else if (toMark)
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

                if ((marked == maxBombs && totalMarked == marked) || opened == poolSize.x * poolSize.y - maxBombs)
                {
                    result.setString("You win :)");
                    screen = END_SCREEN;
                }
                break;
            case END_SCREEN:
                if (event.type == Event::MouseMoved)
                {
                    pixelPos = Mouse::getPosition(window);
                    pos = window.mapPixelToCoords(pixelPos);
                }

                if (event.type == Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        if (buttonExit.isMouseOver(window))
                        {
                            window.close();
                        }
                        else if (buttonReplay.isMouseOver(window))
                        {
                            generatePool(plates, poolSize, maxBombs, hide);
                            screen = IN_GAME_SCREEN;
                            isStarted = false;
                            opened = 0;
                            marked = 0;
                            totalMarked = 0;
                            infBar.setFillColor(Color::Cyan);
                            infBar.setPosition(Vector2f(0, windowSize.y - 50));
                            showtimer.setString("0");
                        }
                    }
                }
                break;
            }
        }

        if (isStarted && screen == IN_GAME_SCREEN)
        {
            time = clock.getElapsedTime();
            showtimer.setString(std::to_string(int(time.asSeconds())));
        }
        else
        {
            clock.restart();
        }

        window.clear();

        switch (screen)
        {
        case MAIN_MENU_SCREEN:
            window.draw(mainMenuContainer);
            window.draw(mainTitle);
            xTextbox.drawTo(window);
            yTextbox.drawTo(window);
            startGameButton.drawTo(window);
            break;
        case IN_GAME_SCREEN:
            for (int i = 0; i < poolSize.x; i++)
            {
                for (int j = 0; j < poolSize.y; j++)
                {
                    window.draw(plates[i][j].sprite);
                }
            }
            window.draw(infBar);
            window.draw(showtimer);
            break;
        case END_SCREEN:
            infBar.setFillColor(Color::Yellow);
            infBar.setSize(Vector2f(32 * poolSize.x, 32 * poolSize.y + 50));
            infBar.setPosition(0, 0);
            window.draw(infBar);
            window.draw(result);
            window.draw(showtimer);
            buttonExit.drawTo(window);
            buttonReplay.drawTo(window);
            break;
        default:
            break;
        }

        window.display();
    }

    return 0;
}