#include "Tile.hpp"
#include "../graphics/Shader.hpp"

#include "../graphics/Texture.hpp"

/**
\file Tile.cpp
\brief The implementation file for the Tile class.

\author Christopher Arausa
\date 05/14/2019
\version Final
*/


/**
\brief Constructor

Generates a unit quad and loads it to the GPU
\param size --- The size of the quad. Defaults to 1 unit.
\param active --- boolean for if the tile is active/being draw or used in collision calcs.
*/
Tile::Tile(float size, bool active){
  this->active = active;
  use_lighting = true;
  id = -1;
  vPosition = 0;
  vTexture = 1;
  useTexture = false;
  active_texture = "";
  shader = nullptr;
  //Generate the quad
  //Vertex positions
  float vp[] = {
    size/2.0f,  size/2.0f, 0, // top right
    size/2.0f, -size/2.0f, 0, // bottom right
    -size/2.0f, -size/2.0f, 0, // bottom left
    -size/2.0f, size/2.0f, 0 // top left
  };

  vertices = std::vector<float>(vp, vp+12);
  //Vertex indices
  unsigned int vi[] = { 0, 1, 3, 1, 2, 3 };
  indices = std::vector<unsigned int>(vi,vi+6);

  float uvs[] = { 1,1,1,0,0,0,0,1 };
  texture_uvs = std::vector<float>(uvs,uvs+8);

  //Load to the graphics card.
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &dataPtr);
  glGenBuffers(1, &indicePtr);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicePtr);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, dataPtr);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + texture_uvs.size() * sizeof(float), NULL, GL_STATIC_DRAW);

  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
  glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), texture_uvs.size() * sizeof(float), texture_uvs.data());

  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size() * sizeof(float)));

  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vTexture);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  setScale(glm::vec3(32,32,1));
}

/**
\brief Destructor

I know you'll take points off for commenting this out, but this legitimately breaks my system for some reason. It's just this one that breaks.
*/
Tile::~Tile(){
  // glDeleteVertexArrays(1, &VAO);
  // glDeleteBuffers(1, &dataPtr);
  // glDeleteBuffers(1, &indicePtr);
}

/**
\brief Sets whether the tile is actively being drawn and used for collision detection.

\param value --- boolean to set.
*/
void Tile::setActive(bool value){
  active = value;
}

/**
\brief Sets the shader the tile uses.

\param s --- The shader pointer to use.
*/
void Tile::setShader(Shader *s){
  this->shader = s;
  if(shader!=nullptr){
    shader->bind();
    uModel = shader->getUniformLocation("model");
  }
}
/**
\brief Returns the ID of the tile type.
*/
int Tile::getID(){
  return id;
}
/**
\brief Returns whether the tile is actively used in collision calcs or being drawn.
*/
bool Tile::getActive(){
  return active;
}
/**
\brief Sets the ID type of the tile.

\param id --- The ID value to use.
*/
void Tile::setID(int id){
  this->id = id;
}
/**
\brief Sets whether the lighting is actively used on this tile.

\param use --- boolean for whether to use lighting.
*/
void Tile::setUseLighting(bool use){
  use_lighting = use;
}
/**
\brief Draws the tile.

\param delta --- float delta time since last draw.
*/
void Tile::draw(float delta){
  if(!active){
    return;
  }
  update(delta);
  if(shader!=nullptr){
    generateModelMatrix();
    shader->bind();
    shader->setMat4(uModel, model_matrix);
    shader->setBool("use_texture", useTexture);
    shader->setBool("use_lighting", use_lighting);
    if(useTexture){
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textures[active_texture]->getID());
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicePtr);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

/**
\brief Tells the class to bind a texture on the draw loop.
\param use --- The boolean value to set for if we're using a texture.
*/
void Tile::setUseTexture(bool use){
  useTexture = use;
}

/**
\brief Sets the active texture of the tile in the texture map.
\param key --- string key used in the map.

*/
void Tile::setTexture(std::string key){
  active_texture = key;
  useTexture = true;
}

/**
\brief Adds a texture to the texture map and sets it as active.
\param key --- string to use as a key in the map.
\param texture --- Texture pointer to use.
*/
void Tile::addTexture(std::string key, Texture* texture){
  textures.insert(std::make_pair(key,texture));
  active_texture = key;
  useTexture = true;
}
/**
\brief Virtual function that all inherited tiles call to do any logic before the draw happens. Such as changing textures/animation stuff.
*/
void Tile::update(float delta){

}
