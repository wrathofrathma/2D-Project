#include "SpacialObject.hpp"
/**
\file SpacialObject.cpp
\brief Implementation of the SpacialObject class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/

/**
\brief Constructor

Default constructor that initializes position and orientation.
*/
SpacialObject::SpacialObject(){
  position = glm::vec3(0);
  orientation = glm::vec3(0);
}
/**
\brief Returns the position of the object.
*/
glm::vec3 SpacialObject::getPosition(){
  return position;
}
/**
\brief Sets the position of the object.
\param pos --- New position of the object.
*/
void SpacialObject::setPosition(glm::vec3 pos){
  position = pos;
}
/**
\brief Sets the oritentation of the object.
\param o --- New orientation of the object.
*/
void SpacialObject::setOrientation(glm::vec3 o){
  orientation = o;
}
/**
\brief Returns the orientation of the object.
*/
glm::vec3 SpacialObject::getOrientation(){
  return orientation;
}
