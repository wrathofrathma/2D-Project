#include "Terrain.hpp"
#include "../AssetManager.hpp"
#include "../Utils/HeightGenerator.hpp"
#include <random>
#include <chrono>


/**
\brief Constructor

This constructor generates the random seed for the world, initializes our tile IDs with world height restrictions, and sets up the empty tile
\param am --- AssetManager class pointer.
*/
Terrain::Terrain(AssetManager *am){
  this->am = am;
  mt19937_64 ra;
  ra.seed(std::chrono::system_clock::now().time_since_epoch().count());
  uniform_int_distribution<unsigned int> gen(0,100000);
  seed = gen(ra);

  if(am!=nullptr)
    init(am);

  empty_tile.setActive(false);
  empty_tile.setID(-1);

  //Initialize our tile id possibilities.
  TILE_BOUNDS.insert(std::make_pair(flesh, std::make_pair(0,15)));
  TILE_BOUNDS.insert(std::make_pair(grass, std::make_pair(WORLD_HEIGHT/2.0,WORLD_HEIGHT)));
  TILE_BOUNDS.insert(std::make_pair(rock, std::make_pair(0,int(WORLD_HEIGHT/2.0))));
  TILE_BOUNDS.insert(std::make_pair(lava, std::make_pair(0,2)));
  TILE_BOUNDS.insert(std::make_pair(stone, std::make_pair(3,WORLD_HEIGHT/3)));

}

/**
\brief Initializes the world and tells us we're ready to generate the world and load textures.

\param am --- AssetManager class pointer.
*/
void Terrain::init(AssetManager *am){
  this->am = am;

  loadTextures();
  world = World(WORLD_HEIGHT);
  generate();

}

/**
\brief This function loads in all textures needed for our tiles and associates them in a map with their ID as a key.
*/
void Terrain::loadTextures(){
  if(am!=nullptr){
    std::vector<Texture*> buffer;


    buffer.push_back(am->getTexture("grass1"));
    textures.insert(std::make_pair(grass, buffer));

    buffer.clear();
    buffer.push_back(am->getTexture("rock1"));
    textures.insert(std::make_pair(rock, buffer));
    buffer.clear();

    buffer.push_back(am->getTexture("stone1"));
    buffer.push_back(am->getTexture("stone2"));
    buffer.push_back(am->getTexture("stone3"));
    textures.insert(std::make_pair(stone, buffer));
    buffer.clear();

    buffer.push_back(am->getTexture("lava1"));
    buffer.push_back(am->getTexture("lava2"));
    textures.insert(std::make_pair(lava, buffer));
    buffer.clear();
    buffer.push_back(am->getTexture("flesh1"));
    buffer.push_back(am->getTexture("flesh2"));
    buffer.push_back(am->getTexture("flesh3"));
    buffer.push_back(am->getTexture("flesh4"));
    buffer.push_back(am->getTexture("flesh5"));
    textures.insert(std::make_pair(flesh, buffer));
  }
}


/**
\brief This function intializes a heightmap generator and generates the terrain based on our seed.
*/
void Terrain::generate(){
  std::mt19937_64 random_gen;
  random_gen.seed(seed);

  HeightGenerator hg;
  hg.setSeed(seed);
  //So our generator.....We're going to do heightmap generation but locking the Z coordinate to 1. This should give some good results.
  //We'll translate the height line to a
  hg.setAmplitude(WORLD_HEIGHT/2.0); //Divided by 2 since the amplitude oscilates between the negative and positive value.
  hg.setOctaves(3);
  hg.setRoughness(0.1);
  std::vector<int> heights(WORLD_WIDTH); //Our vector should be the size of world width.
  for(unsigned int x=0; x<WORLD_WIDTH; x++){
    heights[x] = hg.generateHeight(x,1) + WORLD_HEIGHT/2.0; /// Adding the world height ensures a positive height.
  }
  //Now that we have our list of heights, we need to convert these to tiles. To start let's allocate some space.
  //Doing this in a loop is slightly wasteful, but it saves us some headache later when it comes to inserting by index. It's also not too bad since it's just like, what? at max 256 operations of we have a hella tall world.
  for(unsigned int y=0; y<WORLD_HEIGHT; y++){
    world[y] = WORLD_ROW(WORLD_WIDTH);
  }
  //Now that we have all of our world tiles allocated, we can set the logic for their positions and textures.
  for(unsigned int y=0; y<WORLD_HEIGHT; y++){
    std::vector<TILE_IDS> tiles = getPossibleTiles(y);
    for(unsigned int x=0; x<WORLD_WIDTH; x++){

      //Every tile at least needss a shader and position assignment.
      world[y][x].setPosition(glm::vec3(x*32,y*32,0));
      world[y][x].setShader(am->getShader("Default"));

      //If our tile is considered airspace, we leave the default ID of -1 and skip the rest of the assignment stuff.
      if(heights[x]<y){
        world[y][x].setID(-1);
        world[y][x].setActive(false);
        continue;
      }
      //any top level piece of terrain can be grass.
      else if(heights[x]==y){
        world[y][x].addTexture("grass",am->getTexture("grass1"));
        world[y][x].setID(grass);
      }
      //Anything else we check our tile map.
      else{
        if(tiles.size()==0){
          std::cout << "No tiles" << std::endl;
          world[y][x].setID(-1);
          continue;
        }

        std::uniform_int_distribution<int> tile_pick(0,tiles.size()-1);
        int tid = tile_pick(random_gen);
        tid = tiles[tid];
        world[y][x].setID(tid);
        world[y][x].setActive(true);
        //Now let's generate a texture for this ID.
        std::uniform_int_distribution<int> text_pick(0,textures[TILE_IDS(tid)].size()-1);
        int tex = text_pick(random_gen);
        world[y][x].addTexture("t",textures[TILE_IDS(tid)][tex]);
      }
    }
    tiles.clear();
  }
}


/**
\brief Returns all possible tiles at a given y value.

This is used for terrain generation.

\param y --- The integer y value find tiles for.
*/
std::vector<TILE_IDS> Terrain::getPossibleTiles(int y){
  std::vector<TILE_IDS> tile_ids;
  for(auto it = TILE_BOUNDS.begin(); it!=TILE_BOUNDS.end(); it++){
    //If the y position is between the bounds of the tile, add it to our list.
    if(it->second.second >=y && it->second.first <= y){
      tile_ids.push_back(it->first);
    }
  }
  return tile_ids;
}

/**
\brief Draws all of the tiles in our terrain.
\param delta --- Delta time since last draw.
*/
void Terrain::draw(float delta){
  for(WORLD_ROW wr : world){
    for(Tile tile : wr){
      tile.draw(delta);
    }
  }
}

/**
\brief Deletes terrain at a specific location.

\param x --- Integer location in tile coordinates.
\param y --- Integer location in tile coordinates.
*/
void Terrain::deleteTile(int x, int y){
  if(x <0 || x >= WORLD_WIDTH)
    return;
  else if(y < 0 || y>= WORLD_HEIGHT)
    return;
  else
    world[y][x].setActive(false);
}

/**
\brief Deletes terrain at a specific location.

\param x --- float location in pixel/opengl coordinates.
\param y --- float location in pixel/opengl coordinates.
/*/
Tile Terrain::getTileAtCoord(float x, float y){

  if(x/32 <0 || x/32 >= WORLD_WIDTH)
    return empty_tile;
  else if(y/32 < 0 || y/32>= WORLD_HEIGHT)
    return empty_tile;
  return world[y/32][x/32];
}
/**
\brief Finds the tile at a given tile coordinate.

Returns the found tile or the empty tile.
\param x --- x location in tile coordinates.
\param y --- y location in tile coordinates.
*/
Tile Terrain::getTile(int x, int y){
  if(x <0 || x >= WORLD_WIDTH)
    return empty_tile;
  else if(y < 0 || y>= WORLD_HEIGHT)
    return empty_tile;
  else
    return world[y][x];
}
