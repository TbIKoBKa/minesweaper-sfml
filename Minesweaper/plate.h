#include <SFML/Graphics.hpp>
#include <utility>

class Plate
{
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