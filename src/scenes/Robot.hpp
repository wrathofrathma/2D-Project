#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "../graphics/components/QuaternionObject.hpp"
#include "RobotHead.hpp"
class AssetManager;
class Texture;
#include <vector>
#include <map>

typedef std::vector<Texture*> Animation;

class Robot : QuaternionObject {
  private:
    enum STATE {
      idle,
      falling,
      moving
    };
    GLint vPosition; ///< Shader position of vertex data.
    GLint vColor; ///< Shader position of vertex color.
    GLint vTexture; ///< Shader position of texture

    GLuint VAO; ///< VAO
    GLuint indicePtr; ///< VBO
    GLuint dataPtr; ///< EBO

    AssetManager *am;

    std::map<STATE, Animation> animations;
    RobotHead head;
    void loadTextures();
  public:
    Robot(AssetManager* am=nullptr);
    ~Robot();
    void draw(float delta);
    void init(AssetManager *am);
};

#endif
