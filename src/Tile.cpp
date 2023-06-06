#include "Tile.h"
#include "TextureManager.h"

Tile::Tile()
{
	hidden.setTexture(TextureManager::GetTexture("tile_hidden"));
	revealed.setTexture(TextureManager::GetTexture("tile_revealed"));
	flag.setTexture(TextureManager::GetTexture("flag"));
	isHidden = true;
	isMine = false;
	hasFlag = false;
}

sf::Sprite Tile::GetTile()
{
	if (isHidden)
		return hidden;
	else
		return revealed;
}

void Tile::SetTileValue()
{
	if (isMine)
	{
		value.setTexture(TextureManager::GetTexture("mine"));
	}
	else if (adjacentMines.size() > 0)
	{
		string numMines = std::to_string(adjacentMines.size());
		string file = "number_" + numMines;
		value.setTexture(TextureManager::GetTexture(file));
	}
}

sf::Sprite Tile::GetTileValue()
{
	return value;
}

sf::Sprite Tile::GetFlag()
{
	return flag;
}


void Tile::Reveal()
{
	isHidden = false;
}


