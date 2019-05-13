#ifndef FREE_CAMERA_HPP
#define FREE_CAMERA_HPP

#include "../components/QuaternionObject.hpp"
#include "Camera.hpp"
/**
\file OrthoCamera.hpp
\brief Header file for OrthoCamera.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/8/2019

*/

/**
\class OrthoCamera
\brief Inherited camera class representing FPS style cameras.

This camera is managed using Quaternions.
*/
class OrthoCamera : public Camera {
  public:
    OrthoCamera(unsigned int width, unsigned int height, float FoV);
    ~OrthoCamera();
    void updateView();
    void update();
    void translate(glm::vec2 value);
};

#endif
