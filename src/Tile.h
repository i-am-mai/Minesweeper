#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Tile
{
	sf::Sprite hidden;
	sf::Sprite revealed;
	sf::Sprite value;
	sf::Sprite flag;

public:
	bool isMine;
	bool isHidden;
	bool hasFlag;
	std::vector<Tile*> adjacentMines;

	Tile();
	sf::Sprite GetTile();
	sf::Sprite GetFlag();
	sf::Sprite GetTileValue();
	void SetTileValue();
	void Reveal();
};

