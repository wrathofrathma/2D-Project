#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <vector>

/**
\file Texture.hpp
\brief Header file for Texture.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/8/2019

*/

/**
\enum TextureType
\brief Enum describing whether our texture is a cube map or single texture.
*/

enum TextureType {
  CubeMap,
  Single
};

/**
\class Texture
\brief The texture class handles the loading texture data from files and storing them in an OpenGL texture ID.
*/
class Texture {
  private:
    TextureType type; ///< Defines whether the texture is a cubemap or single texture.
    bool loaded; ///< Defines whether the texture loaded successfully.
    std::vector<sf::Image> texture_images; ///< The actual image data for each texture.
    GLuint texID; ///< The OpenGL texture location ID.
    int width; ///< Width of the texture.
    int height; ///< Height of the texture.
    std::string _filename; ///< Filename of the texture. If multiple files for a cubemap, then they're appended with commas.
  public:
    Texture(std::string filename, bool alpha);
    Texture(std::vector<std::string> filenames);
    ~Texture();
    TextureType getType();
    bool isLoaded();
    GLuint getID();
    std::string getFilename();
};

#endif
