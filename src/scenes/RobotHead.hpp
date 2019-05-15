#ifndef ROBOT_HEAD
#define ROBOT_HEAD

#include "../graphics/components/QuaternionObject.hpp"
class AssetManager;
class Texture;
class Shader;

/**
\file RobotHead.hpp
\brief Header file for RobotHead.cpp

\author Christopher Arausa
\version Final
\date 05/14/2019
*/

/**
\class RobotHead
\brief This class contains all of the logic dealing with the head specifically.

The head is kept as an independent entity to enable a slightly more diverse model with free rotational movement.
*/
class RobotHead : public QuaternionObject{
  private:
    GLint vPosition; ///< Shader position of vertex data.
    GLint vColor; ///< Shader position of vertex color.
    GLint vTexture; ///< Shader position of texture

    GLuint VAO; ///< VAO
    GLuint indicePtr; ///< VBO
    GLuint dataPtr; ///< EBO

    std::vector<float> vertices; ///< Vector containing all of the vertices in our object.
    std::vector<unsigned int> indices; ///< Vector containing vertex indices.
    std::vector<float> texture_uvs; ///< Vector containing our texture coordinates.
    GLuint uModel; ///< Shader uniform position of model matrix.

    Texture* texture; ///< The head's texture. 
    Shader* shader; ///< Shader the head uses.
    static constexpr float head_radius = 11.5; ///< Hard coded value for the radius of the image.
  public:
    RobotHead();
    ~RobotHead();
    void init(AssetManager *am);
    void draw(float delta);
};
#endif
