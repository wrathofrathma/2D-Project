#include "Camera.hpp"
/**
\file Camera.cpp
\brief Implementation of the Camera class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/

/**
\brief Constructor

Sets the default clipping distances to 0.1f, 100.0f and does the initial call to updateProjection()
\param width --- Width of the view window.
\param height --- Height of the view window.
\param FoV --- Field of view value.
*/
Camera::Camera(unsigned int width, unsigned int height, float FoV = 50.0f){
  this->width = width;
  this->height = height;
  this->FoV = FoV;


  clip_near = 0.1;
  clip_far = 200.0f;

  updateProjection();
}

/**
\brief Destructor

Generic destructor with nothing to cleanup
*/
Camera::~Camera(){

}

/**
\brief Updates the size values stored and recalculates the projection matrix.
\param width --- New width of the view window.
\param height --- New height of the view window.
*/
void Camera::resize(unsigned int width, unsigned int height){
  this->width = width;
  this->height = height;
  updateProjection();
}

/**
\brief Updates the projection matrix using glm::perspective.
*/
void Camera::updateProjection(){
  projection = glm::perspective(glm::radians(FoV), float(width) / float(height), clip_near, clip_far);
}

/**
\brief Returns the view matrix
*/
glm::mat4 Camera::getView(){
  return view;
}
/**
\brief Returns the projection matrix.
*/
glm::mat4 Camera::getProjection(){
  return projection;
}
/**
\brief Returns the camera type. FREE or SPHERICAL
*/
CAMERA_TYPE Camera::getType(){
  return type;
}
/**
\brief Applies the camera's projection & view matrices to the shader.
\param shader --- Shader to apply changes to.
*/
void Camera::applyUpdate(Shader *shader){
  shader->bind();
  shader->setMat4(shader->getUniformLocation(VIEW), view);
  shader->setMat4(shader->getUniformLocation(PROJECTION), projection);
  shader->setVec3(shader->getUniformLocation("camera_pos"),position);
}
/**
\brief Sets the FoV value of our projection matrix.
*/
void Camera::setFOV(float fov){
  FoV = fov;
  updateProjection();
}
/**
\brief Sets our near and far clipping values used in our projection matrix.
*/
void Camera::setClipping(float near, float far){
  clip_near = near;
  clip_far = far;
  updateProjection();
}
