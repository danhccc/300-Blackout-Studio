#include "Map.h"
#include "Game.h"
#include "TextureManager.h"




Map::Map()
{
    //wall = TextureManager::LoadTexture("Asset/Wall.png");

}

void Map::LoadMap(int arr[MAP_HEIGHT][MAP_WIDTH])
{
    int type = 0;
    for (int row = 0; row < MAP_HEIGHT; row++)
    {
        for (int column = 0; column < MAP_WIDTH; column++)
        {
            currentMap[row][column] = arr[row][column];
        }
    }
}

void Map::DrawMap()
{
    int type = 0;

    for (int row = 0; row < MAP_HEIGHT; row++)
    {
        for (int column = 0; column < MAP_WIDTH; column++)
        {
            type = currentMap[row][column];
            dest.x = column * 32;
            dest.y = row * 32;

            switch (type)
            {
            case 0:
                //TextureManager::Draw(wall, src, dest);
                break;
            case 1:

            default:
                break;

            }
        }
    }
}

