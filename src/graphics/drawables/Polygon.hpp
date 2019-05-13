#ifndef POLYGON_HPP
#define POLYGON_HPP
#include "../components/Drawable.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

using namespace std;
/**
\file Polygon.hpp
\brief Header file for Polygon.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/8/2019

*/

/**
\class Polygon
\brief The polygon class contains conveience methods for managing and drawing any 2D polygon.
*/
class Polygon : public Drawable {
  protected:

  public:
    Polygon(AssetManager *am);
    Polygon();
    ~Polygon();

    void draw();
    GLuint const getVAO();
    unsigned int getVertexCount();
    void setColor(float r, float g, float b);
    void setColor(std::vector<float> c);
    void setTexture(Texture *tex);
    void setTexture(std::string tex);
};
#endif
