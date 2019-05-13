#ifndef FREE_CAMERA_HPP
#define FREE_CAMERA_HPP

#include "../components/QuaternionObject.hpp"
#include "Camera.hpp"
/**
\file FreeCamera.hpp
\brief Header file for FreeCamera.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/8/2019

*/

/**
\class FreeCamera
\brief Inherited camera class representing FPS style cameras.

This camera is managed using Quaternions.
*/
class FreeCamera : public QuaternionObject, public Camera {
  public:
    FreeCamera(unsigned int width, unsigned int height, float FoV);
    ~FreeCamera();
    void updateView();
    void update();
};

#endif
