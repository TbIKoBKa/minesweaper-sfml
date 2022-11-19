#include "Minesweaper.h"
#include "../../misc/constants.h"

#define GEN_1

Minesweaper::Minesweaper(sf::Font& font) {
    init();

    activeScreen = MAIN_MENU_SCREEN;

    mainMenuContainer = sf::RectangleShape(windowSize);
    mainMenuContainer.setFillColor(sf::Color::White);

    mainTitle = sf::Text("Minesweaper", font, 28);
    mainTitle.setPosition(sf::Vector2f(windowSize.x / 2 - mainTitle.getLocalBounds().width / 2, 10));
    mainTitle.setFillColor(sf::Color::Black);

    subTitle = sf::Text("Enter pool size(10:8 by default)", font, 18);
    subTitle.setPosition(sf::Vector2f(windowSize.x / 2 - subTitle.getLocalBounds().width / 2, 70));
    subTitle.setFillColor(sf::Color::Black);

    xTextbox = Textbox(sf::Vector2f(windowSize.x - 20, 30), 18, sf::Color::White, sf::Color::Black, sf::Color::Black, 2, font, false);
    yTextbox = Textbox(sf::Vector2f(windowSize.x - 20, 30), 18, sf::Color::White, sf::Color::Black, sf::Color::Black, 2, font, false);
    xTextbox.setPosition(sf::Vector2f(10, 100));
    yTextbox.setPosition(sf::Vector2f(10, 140));

    startGameButton = Button("Start", { 90, 40 }, 18, sf::Color::Black, sf::Color::White, font);
    startGameButton.setPosition(sf::Vector2f(windowSize.x / 2 - 45, windowSize.y - 40 - 10));

    infBar = sf::RectangleShape(sf::Vector2f(windowSize.x, 50));
    infBar.setFillColor(sf::Color::Cyan);
    infBar.setPosition(sf::Vector2f(0, windowSize.y - 50));

    buttonExit = Button("Exit", { 90, 40 }, 18, sf::Color::Red, sf::Color::White, font);
    buttonExit.setPosition(sf::Vector2f(32 * poolSize.x / 2 - 50, 32 * poolSize.y - 20));

    buttonReplay = Button("Replay", { 90, 40 }, 18, sf::Color::Green, sf::Color::White, font);
    buttonReplay.setPosition(sf::Vector2f(32 * poolSize.x / 2 - 50, 32 * poolSize.y - 70));

    showtimer.setFont(font);
    showtimer.setCharacterSize(24);
    showtimer.setFillColor(sf::Color::Black);
    showtimer.setPosition(10, 32 * poolSize.y + 10);
    showtimer.setString("0");

    result.setFont(font);
    result.setCharacterSize(50);
    result.setFillColor(sf::Color::Black);
    result.setPosition(30, (float)32 * poolSize.y / 2 - 30);
}

void Minesweaper::init() {
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
} 

void Minesweaper::resetGame() {
    plates = new Plate * [poolSize.x];

    for (int i = 0; i < poolSize.x; i++)
    {
        plates[i] = new Plate[poolSize.y];

        for (int j = 0; j < poolSize.y; j++)
        {
            plates[i][j].sprite.setTexture(hide);
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
        for (int i = 0; i < poolSize.x; i++)
        {
            for (int j = 0; j < poolSize.y; j++)
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
    int b_per_line = (float)maxBombs / (float)poolSize.x;

    int n = 0, lc = 0, bombscount = 0;
    while (bombscount < maxBombs)
    {
        for (int i = 0; i < poolSize.x; i++)
        {
            while (lc < b_per_line)
            {
                if (bombscount >= maxBombs)
                    break;
                n = rand() % poolSize.y;
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

    for (int i = 0; i < poolSize.x; i++)
    {
        for (int j = 0; j < poolSize.y; j++)
        {
            if (!plates[i][j].isBomb)
            {
                for (int iu = i - 1, id = i + 1, jl = j - 1, jr = j + 1; jl < jr + 1; jl++)
                {
                    if (jl >= 0 && jl <= poolSize.y - 1)
                    {
                        if (iu >= 0 && plates[iu][jl].isBomb)
                            plates[i][j].nBombs++;
                        if (plates[i][jl].isBomb)
                            plates[i][j].nBombs++;
                        if (id < poolSize.x && plates[id][jl].isBomb)
                            plates[i][j].nBombs++;
                    }
                }
            }
        }
    }

    activeScreen = IN_GAME_SCREEN;
    isStarted = false;
    opened = 0;
    marked = 0;
    totalMarked = 0;

    infBar.setFillColor(sf::Color::Cyan);
    infBar.setPosition(sf::Vector2f(0, windowSize.y - 50));
    showtimer.setString("0");
}

sf::Vector2f Minesweaper::getWindowSize(sf::Vector2i poolSize) {
    return sf::Vector2f(32 * poolSize.x, 32 * poolSize.y + 50);
}

void Minesweaper::poolEvent(sf::Event &event, sf::RenderWindow& window) {
    if (event.type == sf::Event::Resized)
        windowSize = sf::Vector2f(event.size.width, event.size.height);

    switch (activeScreen) {
    case MAIN_MENU_SCREEN:
        if (event.type == sf::Event::MouseButtonPressed) {
            if (xTextbox.isMouseOver(window)) {
                xTextbox.setSelected(true);
                yTextbox.setSelected(false);
            }
            else if (yTextbox.isMouseOver(window)) {
                yTextbox.setSelected(true);
                xTextbox.setSelected(false);
            }
            else if (startGameButton.isMouseOver(window)) {
                activeScreen = IN_GAME_SCREEN;
                //poolSize = Vector2i(atoi(xTextbox.getText().c_str()), atoi(yTextbox.getText().c_str()));
                //windowSize = getWindowSize(poolSize);
                resetGame();
            }
            else {
                xTextbox.setSelected(false);
                yTextbox.setSelected(false);
            }
        }
        else if (event.type == sf::Event::TextEntered) {
            if (xTextbox.getSelected()) {
                xTextbox.typedOn(event);
            }
            else if (yTextbox.getSelected()) {
                yTextbox.typedOn(event);
            }
        }
        break;
    case IN_GAME_SCREEN:
        if (event.type == sf::Event::MouseButtonPressed)
        {
            isClick = true;
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f pos = window.mapPixelToCoords(pixelPos);

            switch (event.mouseButton.button)
            {
            case sf::Mouse::Left:
                toOpen = true;
                break;
            case sf::Mouse::Right:
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
                                    activeScreen = END_SCREEN;
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
        if (event.type == sf::Event::MouseButtonReleased)
            isClick = false;

        if ((marked == maxBombs && totalMarked == marked) || opened == poolSize.x * poolSize.y - maxBombs)
        {
            result.setString("You win :)");
            activeScreen = END_SCREEN;
        }
        break;
    case END_SCREEN:
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (buttonExit.isMouseOver(window))
                {
                    window.close();
                }
                else if (buttonReplay.isMouseOver(window))
                {
                    resetGame();
                }
            }
        }
        break;
    }
}

void Minesweaper::drawTo(sf::RenderWindow& window) {
    if (isStarted && activeScreen == IN_GAME_SCREEN)
    {
        time = clock.getElapsedTime();
        showtimer.setString(std::to_string(int(time.asSeconds())));
    }
    else
    {
        clock.restart();
    }

    switch (activeScreen)
    {
    case MAIN_MENU_SCREEN:
        window.draw(mainMenuContainer);
        window.draw(mainTitle);
        window.draw(subTitle);
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
        infBar.setFillColor(sf::Color::Yellow);
        infBar.setSize(sf::Vector2f(32 * poolSize.x, 32 * poolSize.y + 50));
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
}