#ifndef QUATERNION_OBJECT_HPP
#define QUATERNION_OBJECT_HPP
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../GraphicsDefinitions.hpp"
#include "SpacialObject.hpp"

/**
\file QuaternionObject.hpp
\brief Header file for QuaternionObject.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/2/2019

*/

/**
\class QuaternionObject
\brief The QuaternionObject class repesents any object in 2D or 3D space that uses quaternions for rotation and translation calculation.
*/

class QuaternionObject : virtual public SpacialObject {

  protected:
    glm::vec3 current_scale; ///< The current accumulated scale of the object.
    glm::mat4 model_matrix; ///< The current model matrix of the object.
  public:
    QuaternionObject();
    virtual ~QuaternionObject();
    virtual void rotate(glm::vec3 rotation);
    virtual void translate(glm::vec3 offset, bool relative=true);
    virtual void scale(glm::vec3 delta);
    glm::mat4 getRotationMatrix();
    glm::quat getRotationQuat();
    glm::mat4 generateModelMatrix();
    glm::vec3 getXAxis();
    glm::vec3 getYAxis();
    glm::vec3 getZAxis();
};
#endif
