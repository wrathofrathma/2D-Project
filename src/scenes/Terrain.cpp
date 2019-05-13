#include "Terrain.hpp"
#include "../AssetManager.hpp"
Terrain::Terrain(AssetManager *am){
  this->am = am;
}

void Terrain::loadTextures(){
  textures.insert(std::make_pair(std::string("flesh1"),am->getTexture("flesh1")));
  textures.insert(std::make_pair(std::string("flesh2"),am->getTexture("flesh2")));
  textures.insert(std::make_pair(std::string("flesh3"),am->getTexture("flesh3")));
  textures.insert(std::make_pair(std::string("flesh4"),am->getTexture("flesh4")));
  textures.insert(std::make_pair(std::string("flesh5"),am->getTexture("flesh5")));
  textures.insert(std::make_pair(std::string("grass1"),am->getTexture("grass1")));
  textures.insert(std::make_pair(std::string("hidden1"),am->getTexture("hidden1")));
  textures.insert(std::make_pair(std::string("hidden2"),am->getTexture("hidden2")));
  textures.insert(std::make_pair(std::string("rock1"),am->getTexture("rock1")));
  textures.insert(std::make_pair(std::string("flesh1"),am->getTexture("flesh1")));

}
