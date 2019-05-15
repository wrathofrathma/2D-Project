#ifndef BEAM_HPP
#define BEAM_HPP
#include <glm/vec2.hpp> // glm::vec2
#include <vector>
#include "../graphics/components/QuaternionObject.hpp"
class Shader;
class Texture;
class AssetManager;

/**
\file Beam.hpp
\brief Header file for Beam.cpp

\author Christopher Arausa
\version Final
\date 05/14/2019
*/

/**
\class Beam
\brief These are the visual representations of the lasers shot out of our robot's head.
*/
class Beam : public QuaternionObject {
  private:
    glm::vec2 start; ///< The starting position of the beam.
    glm::vec2 end; ///< The ending position of the beam.
    float time; ///< The time the beam has been active.

    Shader* shader; ///< The shader the beam will use.

    GLuint VAO; ///< VAO
    GLuint indicePtr; ///< VBO
    GLuint dataPtr; ///< EBO
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    GLuint uModel; ///< Shader uniform position of model matrix.

  public:
    Beam(AssetManager* am, glm::vec2 start, glm::vec2 end);
    void draw(float delta);
    ~Beam();
};

#endif
