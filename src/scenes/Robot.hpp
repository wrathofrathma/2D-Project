#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "../graphics/components/QuaternionObject.hpp"
#include "RobotHead.hpp"
class AssetManager;
class Texture;
class Shader;
#include <vector>
#include <map>

/**
\file Robot.hpp
\brief Header file for Robot.cpp

\author Christopher Arausa
\version Final
\date 05/14/2019
*/

typedef std::vector<Texture*> Animation; ///< Animations are just textures stored in a vector.


/**
\enum ROBOT_STATE
\brief The state of the robot.
*/
enum ROBOT_STATE {
  idle,
  falling,
  moving,
  jumping
};

/**
\class Robot
\brief The robot class implementation. It contains all necessities to draw and move the robot.
*/
class Robot : public QuaternionObject {
  private:

    GLint vPosition; ///< Shader position of vertex data.
    GLint vColor; ///< Shader position of vertex color.
    GLint vTexture; ///< Shader position of texture

    GLuint VAO; ///< VAO
    GLuint indicePtr; ///< VBO
    GLuint dataPtr; ///< EBO
    ROBOT_STATE state; ///< The current state of the robot.
    std::vector<float> vertices; ///< Vector containing all of the vertices in our object.
    std::vector<unsigned int> indices; ///< Vector containing vertex indices.
    std::vector<float> texture_uvs; ///< Vector containing our texture coordinates.
    GLuint uModel; ///< Shader uniform position of model matrix.

    static const int BODY_X = 32; ///< Hard coded size of the textures.
    static const int BODY_Y = 43; ///< Hard coded size of the textures.
    Shader* shader; ///< Shader our robot will use.
    std::map<ROBOT_STATE, Animation> animations; ///< A map of our textures using our robot state as a key.
    int animation_step; ///< The current step in the animation.
    float animation_delta; ///< The animation delta is used to track how long a second is.
    void loadTextures();
    bool headlight; ///< Are our headlights on?
  public:
    Robot();
    ~Robot();
    void draw(float delta);
    void init(AssetManager *am);
    void move(glm::vec2 value);
    void calcHeadRotation(glm::vec2 value);
    glm::vec2 velocity; ///< The robot's current velocity.
    void setState(ROBOT_STATE s);
    ROBOT_STATE getState();
    RobotHead head; ///< The robot's head. 
    void toggleHeadlight();

};

#endif
