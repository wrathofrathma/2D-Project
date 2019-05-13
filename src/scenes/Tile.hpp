#ifndef TILE_HPP
#define TILE_HPP

#include <vector>
#include <map>
#include "../graphics/components/QuaternionObject.hpp"
class Texture;
class Shader;


//Foreground terrain tiles
class Tile : public QuaternionObject {
  private:
    GLint vPosition; ///< Shader position of vertex data.
    GLint vColor; ///< Shader position of vertex color.
    GLint vTexture; ///< Shader position of texture

    GLuint VAO; ///< VAO
    GLuint indicePtr; ///< VBO
    GLuint dataPtr; ///< EBO

    std::vector<float> vertices; ///< Vector containing all of the vertices in our object.
    std::vector<unsigned int> indices; ///< Vector containing vertex indices.
    std::vector<float> colors; ///< Vector containing the (r,g,b) color values for each vertice.
    std::vector<float> texture_uvs; ///< Vector containing our texture coordinates.
    GLuint uModel; ///< Shader uniform position of model matrix.

    std::map<std::string,Texture*> textures;
    Shader* shader;
    std::string active_texture;
    bool useTexture; ///< Tracks whether we're using textures or just colors. Is used to set a uniform in our shader to toggle texure vs color rendering.

  public:
    Tile(float size=1);
    ~Tile();
    void setShader(Shader *s);
    virtual void draw(float delta);
    void setUseTexture(bool u);
    void setTexture(std::string key);
    void addTexture(std::string key, Texture* texture);
    virtual void update(float delta);
};

#endif
