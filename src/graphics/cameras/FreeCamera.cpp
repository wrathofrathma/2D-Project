#include "FreeCamera.hpp"

/**
\brief Constructor

Sets the default position to be 0,0,2, pointing towards the origin.
\param width -- Width of the view window.
\param height -- Height of the view window.
\param FoV --- Field of view value. Default is 50f.
*/
FreeCamera::FreeCamera(unsigned int width, unsigned int height, float FoV = 50.0f) : Camera(width, height, FoV) {

  position = glm::vec3(0, 0, 2);
  orientation = glm::vec3(0,0,0);
  type = FREE;
  updateView();
}

/**
\brief Destructor

Empty destructor. Nothing to really cleanup.
*/
FreeCamera::~FreeCamera(){

}

/**
\brief Updates the view matrix of the camera using quaternion rotations and MVP matrices.
*/
void FreeCamera::updateView(){
  glm::mat4 rotation = getRotationMatrix();
  glm::mat4 translation = glm::mat4(1.0f);
  translation = glm::translate(translation,-position);
  view = rotation * translation;
}

/**
\brief Update function called every render cycle.

Right now it just calls updateView(). 
*/
void FreeCamera::update(){
  updateView();
}
