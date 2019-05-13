#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // value ptr
#include "../../AssetManager.hpp"
#include "QuaternionObject.hpp"
#include "../Material.hpp"

/**
\file Drawable.hpp
\brief Header file for Drawable.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 4/20/2019

*/


/**
\class Drawable
\brief The Drawable is a component class for anything that wants to be drawn in our primary draw loop.
*/

class Drawable : virtual public QuaternionObject {
  protected:
    GLint vPosition; ///< Shader position of vertex data.
    GLint vColor; ///< Shader position of vertex color.
    GLint vNormal; ///< Shader position of vertex normal.
    GLint vTexture; ///< Shader position of texture

    std::vector<float> vertices; ///< Vector containing all of the vertices in our object.
    std::vector<unsigned int> indices; ///< Vector containing vertex indices.
    std::vector<float> normals; ///< Vector containing vertex normals.
    std::vector<float> colors; ///< Vector containing the (r,g,b) color values for each vertice.
    std::vector<float> texture_uvs; ///< Vector containing our texture coordinates.

    Material material; ///< Our object's material.
    Shader *shader; ///< Pointer to whatever shader we end up using.

    std::vector<Texture*> textures; ///< Vector containing texture pointers to any texture associated with our object.
    GLuint VAO; ///< VAO
    GLuint indicePtr; ///< VBO
    GLuint dataPtr; ///< EBO

    AssetManager *asset_manager; ///< AssetManager pointer for loading things if we must.
    GLuint uModel; ///< Shader uniform position of model matrix.

    bool useTexture; ///< Tracks whether we're using textures or just colors. Is used to set a uniform in our shader to toggle texure vs color rendering.
    bool lighting_on; ///< Determines if we apply lighting to this drawable.
    bool update; ///< Signals that we need to update the graphics card.
  public:
    Drawable();
    Drawable(AssetManager *am);
    virtual ~Drawable();
    void setAssetManager(AssetManager *am);
    //Draw needs to be virtual to make thing simple.
    virtual void draw() = 0;

    //Almost every object will use the same updateGraphicsCard function, so let's stick it her but make it virtual in case we need something new later.
    virtual void updateGraphicsCard();
    void setUseTexture(bool);

    //Getter specifically for our copy constructors.
    AssetManager *getAssetManager();

    //Set methods. Either vectors or floats + count to generate vectors.
    void setVertices(std::vector<float> v);
    void setIndices(std::vector<unsigned int> ind);
    void setTextureUVs(std::vector<float> uvs);
    void setNormals(std::vector<float> norms);
    void setColors(std::vector<float> c);

    void setVertices(float* v, unsigned int count);
    void setIndices(unsigned int* ind, unsigned int count);
    void setTextureUVs(float* uvs, unsigned int count);
    void setNormals(float* norms, unsigned int count);
    void setColors(float* c, unsigned int count);

    virtual void addTexture(std::vector<Texture*> texts);
    virtual void addTexture(Texture* text);
    void setMaterial(Material mat);
    Material& getMaterial();
    void setLightingOn(bool onoff);
    bool getLightingOn();
    void setShader(const std::string &name);
    void setShader(Shader *s);
    void init();
    void cleanup();
    virtual void initDraw();
    void loadMaterial();
};

#endif
