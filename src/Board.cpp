#include <fstream>
#include <iostream>
#include "Board.h"
#include "TextureManager.h"
#include "Random.h"

using std::ifstream;
using std::endl;

Board::Board()
{
	ifstream file("boards/config.cfg");

	string buffer;

	if (file.is_open())
	{
		getline(file, buffer);
		numCols = stoi(buffer);
		getline(file, buffer);
		numRows = stoi(buffer);
		getline(file, buffer);
		numMines = stoi(buffer);

		file.close();
	}

	tiles = vector<vector<Tile>>(numRows, vector<Tile>(numCols));

	vector<int> mines;
	Random::RandomSample(0, numCols* numRows, 50, mines);
	for (int i = 0; i < numMines; i++)
	{
		int row = mines[i] / numCols;
		int col = mines[i] % numCols;
		tiles[row][col].isMine = true;
	}
	numFlags = numMines;

	smile.setTexture(TextureManager::GetTexture("face_happy"));

	smile_x = (numCols * 32 - smile.getGlobalBounds().width) / 2;
	smile_y = numRows * 32;

	SetAdjacentMines(mines);
	SetTileValues();
}

Board::Board(string boardFile)
{
	ifstream file("boards/config.cfg");

	string buffer;

	if (file.is_open())
	{
		getline(file, buffer);
		numCols = stoi(buffer);
		getline(file, buffer);
		numRows = stoi(buffer);
		file.close();
	}

	smile.setTexture(TextureManager::GetTexture("face_happy"));

	smile_x = (numCols * 32 - smile.getGlobalBounds().width) / 2;
	smile_y = numRows * 32;

	numMines = 0;
	vector<int> mines;

	ifstream board("boards/" + boardFile + ".brd");
	if (board.is_open())
	{
		char c;
		int row = 0;
		int col = 0;
		while (board.get(c))
		{
			tiles.push_back(vector<Tile>());
			if (c == '\n')
			{
				col = 0;
				row++;
				continue;
			}
			Tile newTile;
			if (c == '1')
			{
				newTile.isMine = true;
				numMines++;
				mines.push_back(row * numCols + col);
			}
			tiles[row].push_back(newTile);
			col++;
		}
		SetAdjacentMines(mines);
		SetTileValues();
		numFlags = numMines;
		board.close();
	}
}

void Board::Reset()
{
	tiles.clear();
	numCols = 0;
	numRows = 0;
	numMines = 0;
	numFlags = 0;
}

void Board::SetAdjacentMines(vector<int>& mines)
{
	for (unsigned int i = 0; i < mines.size(); i++)
	{
		int row = mines[i] / numCols;
		int col = mines[i] % numCols;
		for (int j = row - 1; j <= row + 1; j++)
		{
			for (int k = col - 1; k <= col + 1; k++)
			{
				if (j >= 0 && j < numRows && k >= 0 && k < numCols)
				{
					if (tiles[j][k].isMine)
					{
						continue;
					}
					tiles[j][k].adjacentMines.push_back(&tiles[row][col]);
				}
			}
		}
	}
}

void Board::SetTileValues()
{
	for (int i = 0; i < numRows; i++)
		for (int j = 0; j < numCols; j++)
			tiles[i][j].SetTileValue();
}

void Board::Draw(sf::RenderWindow& window)
{
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			sf::Sprite tile = tiles[i][j].GetTile();
			float x_pos = tile.getLocalBounds().width * j;
			float y_pos = tile.getLocalBounds().height * i;

			tile.setPosition(x_pos, y_pos);
			window.draw(tile);
			if (!tiles[i][j].isHidden && (tiles[i][j].isMine || tiles[i][j].adjacentMines.size() > 0))
			{
				sf::Sprite tileValue = tiles[i][j].GetTileValue();
				tileValue.setPosition(x_pos, y_pos);
				window.draw(tileValue);
			}
			if (tiles[i][j].isHidden && tiles[i][j].hasFlag)
			{
				sf::Sprite flag = tiles[i][j].GetFlag();
				flag.setPosition(x_pos, y_pos);
				window.draw(flag);
			}
			if (inDebug)
			{
				if (tiles[i][j].isMine)
				{
					sf::Sprite tileValue = tiles[i][j].GetTileValue();
					tileValue.setPosition(x_pos, y_pos);
					window.draw(tileValue);
				}
			}
		}
	}
	DrawFlags(window);
	DrawButtons(window);
}

void Board::DrawButtons(sf::RenderWindow& window)
{

	float y_pos = smile_y;
	float x_pos = smile_x;

	smile.setPosition(x_pos, y_pos);
	window.draw(smile);

	x_pos += 2 * smile.getGlobalBounds().width;

	sf::Sprite button;

	button.setTexture(TextureManager::GetTexture("debug"));
	button.setPosition(x_pos, y_pos);
	window.draw(button);

	x_pos += button.getGlobalBounds().width;

	for (int i = 1; i <= 3; i++)
	{
		button.setTexture(TextureManager::GetTexture("test_" + std::to_string(i)));
		button.setPosition(x_pos, y_pos);
		window.draw(button);
		x_pos += button.getGlobalBounds().width;
	}
}

void Board::DrawFlags(sf::RenderWindow& window)
{
	float y = window.getSize().y - 100;
	float x = 0;

	sf::Sprite number;
	number.setTexture(TextureManager::GetTexture("digits"));

	if (numFlags < 0)
	{
		number.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
		number.setPosition(x, y);
		window.draw(number);
	}

	x += 21;

	int temp = numFlags >= 0 ? numFlags : -1 * numFlags;
	vector<int> digits;
	while (temp > 0)
	{
		int digit = temp % 10;
		digits.insert(digits.begin(), digit);
		temp /= 10;
	}

	for (int i = 3 - digits.size(); i > 0; i--)
	{
		number.setTextureRect(sf::IntRect(0, 0, 21, 32));
		number.setPosition(x, y);
		window.draw(number);
		x += 21;
	}

	for (unsigned int i = 0; i < digits.size(); i++)
	{
		number.setTextureRect(sf::IntRect(21 * digits[i], 0, 21, 32));
		number.setPosition(x, y);
		window.draw(number);
		x += 21;
	}
}

void Board::CheckButton(float x, float y, Board& board)
{
	float x_pos = smile_x;
	float y_pos = smile_y;

	float width = smile.getGlobalBounds().width;
	float height = smile.getGlobalBounds().height;

	if (x >= smile_x && x < (smile_x + width) && y >= smile_y && y < (smile_y + height))
	{
		Reset();
		board = Board();
	}
	else
	{
		x_pos += 2 * width;

		if (x >= x_pos && x < (x_pos + width) && y >= y_pos && y < (y_pos + height) && !gameOver)
		{
			inDebug = !inDebug;
		}

		x_pos += width;

		for (int i = 1; i <= 3; i++)
		{
			if (x >= x_pos && x < (x_pos + width) && y >= y_pos && y < (y_pos + height))
			{
				Reset();
				board = Board("testboard" + std::to_string(i));
			}
			x_pos += width;
		}
	}
}

void Board::RevealTiles(int row, int col)
{
	int numAdjMines = tiles[row][col].adjacentMines.size();

	if (numAdjMines > 0)
	{
		tiles[row][col].Reveal();
		return;
	}

	for (int i = row - 1; i <= row + 1; i++)
	{
		for (int j = col - 1; j <= col + 1; j++)
		{
			if (i < 0 || i >= numRows || j < 0 || j >= numCols)
			{
				continue;
			}
			if (!tiles[i][j].isHidden || tiles[i][j].hasFlag)
			{
				continue;
			}
			else
			{
				tiles[i][j].Reveal();
				RevealTiles(i, j);
			}
		}
	}
}

void Board::RevealMines()
{
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (tiles[i][j].isMine)
				tiles[i][j].Reveal();
		}
	}
}

void Board::Flag(int tileRow, int tileCol)
{
	tiles[tileRow][tileCol].hasFlag = !tiles[tileRow][tileCol].hasFlag;
	numFlags = tiles[tileRow][tileCol].hasFlag ? --numFlags : ++numFlags;
}

void Board::Lose()
{
	RevealMines();
	smile.setTexture(TextureManager::GetTexture("face_lose"));
	gameOver = true;
}

void Board::CheckWin()
{
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (tiles[i][j].isHidden && !tiles[i][j].isMine)
				return;
			else if (tiles[i][j].isMine && !tiles[i][j].isHidden)
				return;
		}
	}
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (tiles[i][j].isMine)
			{
				numFlags = tiles[i][j].hasFlag == true ? numFlags : --numFlags;
				tiles[i][j].hasFlag = true;
			}
		}
	}
	smile.setTexture(TextureManager::GetTexture("face_win"));
	gameOver = true;
}
