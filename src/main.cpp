#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "TextureManager.h"
#include "Board.h"
#include "Tile.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::vector;


int main()
{
	Board board;

	sf::RenderWindow window(sf::VideoMode(board.numCols * 32, board.numRows * 32 + 100), "Minesweeper");
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::MouseButtonPressed:
				{
					int tileCol = event.mouseButton.x / 32;
					int tileRow = event.mouseButton.y / 32;

					if (event.mouseButton.button == sf::Mouse::Left)
					{
						if (tileCol < board.numCols && tileRow < board.numRows)
						{
							if (board.gameOver || board.tiles[tileRow][tileCol].hasFlag)
								continue;
							else if (board.tiles[tileRow][tileCol].isMine)
								board.Lose();
							else 
							{
								board.RevealTiles(tileRow, tileCol);
								board.CheckWin();
							}
						}
						else
							board.CheckButton(event.mouseButton.x, event.mouseButton.y, board);
					}
					else if (event.mouseButton.button == sf::Mouse::Right)
					{
						if (board.gameOver)
							continue;
						else if (tileCol < board.numCols && tileRow < board.numRows)
							board.Flag(tileRow, tileCol);
					}
				}
				default:
					break;
			}
		}

		window.clear(sf::Color::Black);
		board.Draw(window);
		window.display();

	}
	TextureManager::Clear();
	return 0;
}