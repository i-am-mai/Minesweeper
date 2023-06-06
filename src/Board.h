#pragma once
#include <vector>
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using std::vector;
using std::string;

class Board
{
private:
	void SetAdjacentMines(vector<int>& mines);
	void DrawButtons(sf::RenderWindow& window);
	void DrawFlags(sf::RenderWindow& window);


	int numMines = 0;
	bool inDebug = false;
	float smile_x = 0.f;
	float smile_y = 0.f;
	sf::Sprite smile;

public:
	vector<vector<Tile>> tiles;
	bool gameOver = false;
	int numCols = 0;
	int numRows = 0;
	int numFlags = 0;

	Board();
	Board(string boardFile);
	
	void Reset();
	void SetTileValues();
	void Draw(sf::RenderWindow& window);
	void RevealTiles(int row, int col);
	void RevealMines();
	void Flag(int tileRow, int tileCol);

	void CheckWin();
	void Lose();

	void CheckButton(float x, float y, Board& board);
};

