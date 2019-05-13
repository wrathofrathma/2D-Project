#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include <string>
#include <map>
#include <vector>


#include "Tile.hpp"

//Forward delcarations
class AssetManager;
class Texture;

typedef std::vector<std::vector<Tile>> World;

class Terrain {
    std::map<std::string, Texture*> textures; ///< Our terrain textures.
    unsigned int seed;
    void loadTextures();
    AssetManager *am;
    World world;
    Tile t;

  public:
    Terrain(AssetManager* am = nullptr);
    void generate();
    void init(float size);
    void draw(float delta);
    void init(AssetManager* am);
};
#endif
