#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include <string>
#include <map>


//Forward delcarations
class AssetManager;
class Texture;


class Terrain {
    std::map<std::string, Texture*> textures; ///< Our terrain textures.

    void loadTextures();
    AssetManager *am;
  public:
    Terrain(AssetManager*);
};
#endif
