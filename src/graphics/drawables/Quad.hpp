#ifndef QUAD_HPP
#define QUAD_HPP

#include "Polygon.hpp"
/**
\file Quad.hpp
\brief Header file for Quad.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/8/2019

*/

/**
\class Quad
\brief A convenience class for drawing 2D quads on the screen.

This class inherits the Polygon class for most of the functionality.
*/
class Quad : public Polygon {
  private:
    glm::vec4 dimensions; ///< A vector containing the length of each side of our quad.
    void generateQuad();

  public:
    Quad();
    Quad(AssetManager *am, float x=1.0, float y=1.0, float z=1.0, float w=1.0);
    void loadQuad(AssetManager *am, float x=1.0, float y=1.0, float z=1.0, float w=1.0);
    ~Quad();
    void setDimensions(glm::vec4 dims);
};

#endif
