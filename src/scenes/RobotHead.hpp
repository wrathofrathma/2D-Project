#ifndef ROBOT_HEAD
#define ROBOT_HEAD

#include "../graphics/components/QuaternionObject.hpp"
class AssetManager;
class Texture;
class Shader;

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

    Texture* texture;
    Shader* shader;
    static constexpr float head_radius = 11.5;
  public:
    RobotHead();
    ~RobotHead();
    void init(AssetManager *am);
    void draw(float delta);
};
#endif
