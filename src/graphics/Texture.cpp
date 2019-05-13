#include "Texture.hpp"

using namespace std;
/**
\file Texture.cpp
\brief Implementation of the Texture class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/


/**
\brief Constructor

Loads a single image texture from file into an OpenGL texture ID.
\param filename --- File path of the texture to load.
*/
Texture::Texture(std::string filename, bool alpha){
  type = Single;
  texture_images.emplace_back(sf::Image());
  loaded = texture_images[0].loadFromFile(filename);
  _filename = filename;
  if(loaded){
    texture_images[0].flipVertically();
    glGenTextures(1, &texID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    if(!alpha){
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else{
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_images[0].getSize().x, texture_images[0].getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_images[0].getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

    height = texture_images[0].getSize().y;
    width = texture_images[0].getSize().x;
  }
}
/**
\brief Constructor

Loads a vector of filenames into a cubemap. If there aren't 6 images, then it sets the loaded bool to false and returns.
If it successfully loads all 6, then they're stored in a CubeMap texture ID.
\param filenames --- A vector of 6 filepaths.
*/
Texture::Texture(std::vector<std::string> filenames){
  type = CubeMap;
  loaded = false;
  _filename = "";
  if(filenames.size()!=6)
    return;

  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
  for(unsigned int i = 0; i<6; i++){
    _filename+=","+filenames[i];
    texture_images.emplace_back(sf::Image());
    loaded = texture_images[i].loadFromFile(filenames[i]);
    if(!loaded)
      return;
    //texture_images[i].flipVertically();

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture_images[i].getSize().x, texture_images[i].getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_images[i].getPixelsPtr());
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


/**
\brief Returns the type of texture.
*/
TextureType Texture::getType(){
  return type;
}
/**
\brief Destructor

Releases texture data on the graphics card.
*/
Texture::~Texture(){
  if(loaded)
    glDeleteTextures(1, &texID);
}
/**
\brief Returns whether the texture has successfully loaded or not.
*/
bool Texture::isLoaded(){
  return loaded;
}
/**
\brief Returns the texture's OpenGL texture ID.
*/
GLuint Texture::getID(){
  return texID;
}
/**
\brief Returns the filename of the texture.
*/
std::string Texture::getFilename(){
  return _filename;
}
