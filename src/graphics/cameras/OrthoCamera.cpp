#include "OrthoCamera.hpp"

/**
\brief Constructor

Sets the default position to be 0,0,2, pointing towards the origin.
\param width -- Width of the view window.
\param height -- Height of the view window.
\param FoV --- Field of view value. Default is 50f.
*/
OrthoCamera::OrthoCamera(unsigned int width, unsigned int height, float FoV = 50.0f) : Camera(width, height, FoV) {

  position = glm::vec3(0, 0, 1);
  updateView();
}

/**
\brief Destructor

Empty destructor. Nothing to really cleanup.
*/
OrthoCamera::~OrthoCamera(){

}

void OrthoCamera::translate(glm::vec2 value){
  position += glm::vec3(value, 0);
}
/**
\brief Updates the view matrix of the camera using quaternion rotations and MVP matrices.
*/
void OrthoCamera::updateView(){
  view = glm::lookAt(position, position + glm::vec3(0,0,-1.0f), glm::vec3(0,1,0));
}

/**
\brief Update function called every render cycle.

Right now it just calls updateView().
*/
void OrthoCamera::update(){
  updateView();
}
