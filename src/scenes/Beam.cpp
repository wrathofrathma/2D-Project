#include "Beam.hpp"
#include <iostream>
#include "../AssetManager.hpp"
#include "../graphics/Texture.hpp"
#include "../graphics/Shader.hpp"

/**
\file Beam.cpp
\brief Implementation file for the Beam class.

\author Christopher Arausa
\version Final
\date 05/14/2019
*/


/**
\brief Constructor

\param am --- The asset manager used to retrieve shader data.
\param start --- start position of the beam
\param end --- end position of the beam.
*/
Beam::Beam(AssetManager* am, glm::vec2 start, glm::vec2 end){
  this->start = start;
  this->end = end;

  shader = am->getShader("Beam");
  shader->bind();
  uModel = shader->getUniformLocation("model");
  time = 0;
  //translate(glm::vec3(start.x, start.y,0),false);
  setPosition(glm::vec4(start.x, start.y,0,0));
  float xoff = end.x - start.x;
  float yoff = end.y - start.y;
  float vp[] = {
    0,0,0,
    (end.x-start.x), (end.y-start.y), 0
  };
  float inds[] = {
    0,1
  };
  vertices = std::vector<float>(vp,vp+6);
  indices = std::vector<unsigned int>(inds, inds+2);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &dataPtr);
  glGenBuffers(1, &indicePtr);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicePtr);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, dataPtr);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(),GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

/**
\brief Destructor

cleans up the gpu memory
*/

Beam::~Beam(){
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &dataPtr);
  glDeleteBuffers(1, &indicePtr);
}

/**
\brief Draws our beam. It's just a basic line with a model matrix transformation

\param delta --- float delta time used to heat the beam up.
*/

void Beam::draw(float delta){
  time+=delta;
  if(shader!=nullptr){
    generateModelMatrix();
    shader->bind();
    shader->setMat4(uModel, model_matrix);
    shader->setFloat("time", time);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicePtr);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}
