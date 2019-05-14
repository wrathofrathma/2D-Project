#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include <string>
#include <map>
#include <vector>


#include "Tile.hpp"

//Forward delcarations
class AssetManager;
class Texture;

enum TILE_IDS{
  flesh,
  grass,
  rock
};


static constexpr int WORLD_WIDTH = 128;
static constexpr int WORLD_HEIGHT = 64;

typedef std::vector<Tile> WORLD_ROW;
typedef std::vector<WORLD_ROW> World;
class Terrain {
    std::map<TILE_IDS, std::vector<Texture*>> textures; ///< Our terrain textures.
    unsigned int seed;
    void loadTextures();
    AssetManager *am;
    World world;
    std::map<TILE_IDS, std::pair<int,int>> TILE_BOUNDS;
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
    Tile empty_tile;

};




#endif
