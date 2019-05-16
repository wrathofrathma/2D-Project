#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include <string>
#include <map>
#include <vector>


#include "Tile.hpp"
/**
\file Terrain.hpp
\brief Header file for Terrain.cpp

\author Christopher Arausa
\version Final
\date 05/14/2019
*/


//Forward delcarations
class AssetManager;
class Texture;
/**
\enum TILE_IDS
\brief These are just the values of the different tiles we have in the game. There aren't many but it's easy to expand.
*/
enum TILE_IDS{
  flesh=0,
  grass=1,
  rock=2,
  stone=3,
  lava=4,
  black=5
};


static constexpr int WORLD_WIDTH = 128; ///< Static world width.
static constexpr int WORLD_HEIGHT = 64; ///< Static world height.

typedef std::vector<Tile> WORLD_ROW; ///< Typedef for a world row. It's just a vector of tiles.
typedef std::vector<WORLD_ROW> World; ///< Each world Y value is a world row. So naturally a world is just a vector of rows.

/**
\class Terrain
\brief The terrain class handles all terrain generation logic and world tile interaction logic.
*/
class Terrain {
    std::map<TILE_IDS, std::vector<Texture*>> textures; ///< Terrain textures mapped to their ID as a key.
    unsigned int seed; ///< The world seed.
    void loadTextures();
    AssetManager *am; ///< The asset manager class to retrieve textures from.
    World world; ///< Our world type.
    std::map<TILE_IDS, std::pair<int,int>> TILE_BOUNDS; ///< A map of the height values specific tiles can spawn.
  public:
    Terrain(AssetManager* am = nullptr);
    void generate();
    void init(float size);
    void draw(float delta);
    void init(AssetManager* am);
    std::vector<TILE_IDS> getPossibleTiles(int y);
    Tile getTileAtCoord(float x, float y);
    Tile getTile(int x, int y);
    void deleteTile(int x, int y);
    Tile empty_tile; ///< Empty tile that is returned when one isn't found.

};




#endif
