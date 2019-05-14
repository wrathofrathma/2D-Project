#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "../graphics/components/QuaternionObject.hpp"
#include "RobotHead.hpp"
class AssetManager;
class Texture;
class Shader;
#include <vector>
#include <map>

typedef std::vector<Texture*> Animation;

enum ROBOT_STATE {
  idle,
  falling,
  moving,
  jumping
};

class Robot : public QuaternionObject {
  private:

    GLint vPosition; ///< Shader position of vertex data.
    GLint vColor; ///< Shader position of vertex color.
    GLint vTexture; ///< Shader position of texture

    GLuint VAO; ///< VAO
    GLuint indicePtr; ///< VBO
    GLuint dataPtr; ///< EBO
    ROBOT_STATE state;
    std::vector<float> vertices; ///< Vector containing all of the vertices in our object.
    std::vector<unsigned int> indices; ///< Vector containing vertex indices.
    std::vector<float> texture_uvs; ///< Vector containing our texture coordinates.
    GLuint uModel; ///< Shader uniform position of model matrix.

    static const int BODY_X = 32;
    static const int BODY_Y = 43;
    Shader* shader;
    std::map<ROBOT_STATE, Animation> animations;
    int animation_step;
    float animation_delta;
    RobotHead head;
    void loadTextures();
  public:
    Robot();
    ~Robot();
    void draw(float delta);
    void init(AssetManager *am);
    void move(glm::vec2 value);
    void calcHeadRotation(glm::vec2 value);
    glm::vec2 velocity;
    void setState(ROBOT_STATE s);
    ROBOT_STATE getState();
};

#endif
