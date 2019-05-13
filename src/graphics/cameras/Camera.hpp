#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Shader.hpp"
#include "../components/SpacialObject.hpp"

/**
\file Camera.hpp
\brief Header file for Camera.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/8/2019

*/

/**
\enum CAMERA_TYPE
\brief Enum describing whether our camera is free or orbital.
*/
enum CAMERA_TYPE {
  FREE,
  SPHERICAL
};

/**
\class Camera
\brief Base level class for any camera objects.

Manages the projection and view matrix as well as position of the camera.
*/

class Camera : virtual public SpacialObject {
  protected:
    CAMERA_TYPE type; ///< Enum containing the type of camera, free or spherical/orbital.

    float FoV; ///< Field of view.
    float clip_near; ///< Near clipping distance.
    float clip_far; ///< Far clipping distance.
    unsigned int height; ///< Height of the window, used for setting the projection matrix.
    unsigned int width; ///< Width of the window, used for setting the projection matrix.
    glm::mat4 view; ///< Our view matrix. We don't want to calculate this more than once a frame.
    glm::mat4 projection; ///< Our projection matrix. We don't want to calculate this more than once a frame.
  public:
    Camera(unsigned int width, unsigned int height, float FoV);
    virtual ~Camera();
    void setFOV(float fov);
    void setClipping(float near, float far);
    void applyUpdate(Shader *shader);
    glm::mat4 getView();
    glm::mat4 getProjection();
    void resize(unsigned int width, unsigned int height);
    void updateProjection();
    virtual void updateView() = 0;
    virtual void update() = 0;
    CAMERA_TYPE getType();
};

#endif
