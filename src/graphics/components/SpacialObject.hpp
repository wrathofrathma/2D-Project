#ifndef SPACIAL_OBJECT_HPP
#define SPACIAL_OBJECT_HPP
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

/**
\file SpacialObject.hpp
\brief Header file for SpacialObject.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/8/2019

*/

/**
\class SpacialObject
\brief The SpacialObject class is a component class for any object that contains a position and orientation.
*/
class SpacialObject {
  protected:
    glm::vec3 position; ///< Vector containing current world coordinates.
    glm::vec3 orientation; ///< Vector containing orientation/rotation applied to the object.
  public:
    SpacialObject();
    glm::vec3 getPosition();
    glm::vec3 getOrientation();
    void setPosition(glm::vec3 pos);
    void setOrientation(glm::vec3 o);

};

#endif
