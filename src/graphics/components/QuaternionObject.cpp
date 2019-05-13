#include "QuaternionObject.hpp"
/**
\file QuaternionObject.cpp
\brief Implementation of the QuaternionObject class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/

/**
\brief Constructor

Initializes and sets default values for position, orientation, scale, and the model matrix.
*/
QuaternionObject::QuaternionObject(){
  position = glm::vec3(0);
  orientation = glm::vec3(0);
  current_scale = glm::vec3(1.0f);
  model_matrix = glm::mat4(1.0f);
}
/**
\brief Destructor

Virtual destructor with nothing to cleanup.
*/
QuaternionObject::~QuaternionObject(){

}

/**
\brief Adds rotation value to our object's rotation.
\param rotation --- glm::vec3 containing the rotation in radians to apply to each axis.
*/
void QuaternionObject::rotate(glm::vec3 rotation){
  orientation+=rotation;
}
/**
\brief Returns the current rotation matrix
*/
glm::mat4 QuaternionObject::getRotationMatrix(){
  return glm::mat4_cast(getRotationQuat());
}
/**
\brief Returns the current rotation quaternion
*/
glm::quat QuaternionObject::getRotationQuat(){
  //generate quaternions based on our Yaw Pitch and Roll
  glm::quat qyaw = glm::angleAxis(orientation.x, glm::vec3(0,1,0));
  glm::quat qpitch = glm::angleAxis(orientation.y, glm::vec3(1,0,0));
  glm::quat qroll = glm::angleAxis(orientation.z, glm::vec3(0,0,1));
  //Generate the total accumulated rotation
  glm::quat orientation = qroll * qpitch * qyaw;
  //Normalize it/make it length 1
  return glm::normalize(orientation);
}
/**
\brief Determines what the object's X axis is relative to itself.

It returns a vector containing the direction the X axis is in.
*/
glm::vec3 QuaternionObject::getXAxis(){
  glm::quat rotation = getRotationQuat();
  return glm::conjugate(rotation) * glm::vec3(1, 0, 0);
}
/**
\brief Determines what the object's Y axis is relative to itself.

It returns a vector containing the direction the Y axis is in.
*/
glm::vec3 QuaternionObject::getYAxis(){
  glm::quat rotation = getRotationQuat();
  return glm::conjugate(rotation) * glm::vec3(0, 1, 0);
}
/**
\brief Determines what the object's Z axis is relative to itself.

It returns a vector containing the direction the Z axis is in.
*/
glm::vec3 QuaternionObject::getZAxis(){
  glm::quat rotation = getRotationQuat();
  return glm::conjugate(rotation) * glm::vec3(0, 0, 1);
}

/**
\brief Translates the object according to the passed vector.
\param offset --- The vector containing the direction to translate our object.
\param relative --- Boolean determining if we translate in world coordinates or relative to the object.
*/
void QuaternionObject::translate(glm::vec3 offset, bool relative){
  if(relative){
    position+=getXAxis() * offset.x;
    position+=getYAxis() * offset.y;
    position+=getZAxis() * offset.z;
  }
  else{
    position+=offset;
  }
}

/**
\brief Generates and returns the object's current model matrix.
*/
glm::mat4 QuaternionObject::generateModelMatrix(){
  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
  glm::mat4 rotationMatrix = getRotationMatrix();
  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), current_scale);
  model_matrix = translationMatrix * rotationMatrix * scaleMatrix;
  return model_matrix;
}

/**
\brief Multiplicatively scales the object size.
\param s --- The scale to apply to the object.
*/ 
void QuaternionObject::scale(glm::vec3 s){
  current_scale*=s;
}
