#include "RobotHead.hpp"
#include <cmath>
#include "../AssetManager.hpp"
#include "../graphics/Shader.hpp"
#include "../graphics/Texture.hpp"

RobotHead::RobotHead(){
  vPosition = 0;
  vTexture = 1;
  shader = nullptr;
  int divisions = 24;
  float angle_mult = (2*M_PI)/divisions;

  float verts[(divisions+1)*3];
  float uvs[(divisions+1)*2];
  verts[0] = 0;
  verts[1] = 0;
  verts[2] = 0;
  uvs[0] = 0.5;
  uvs[1] = 0.5;
  for(int i=0; i<divisions; i++){
    verts[(i+1)*3] = head_radius * cos(float(i)*angle_mult);
    verts[(i+1)*3+1] = head_radius * sin(float(i)*angle_mult);
    verts[(i+1)*3+2] = 0;
    uvs[(i+1)*2] = 0.5 * cos(float(i)*angle_mult) + 0.5;
    uvs[(i+1)*2+1] = 0.5 * sin(float(i)*angle_mult) + 0.5;
  }
  vertices = std::vector<float>(verts, verts+(divisions+1)*3);
  unsigned int inds[divisions+2];
  for(unsigned int i=0; i<(divisions+1); i++)
    inds[i] = i;
  inds[divisions+1] = 1;//Need to loop back.
  indices = std::vector<unsigned int>(inds, inds+(divisions+2));

  texture_uvs = std::vector<float>(uvs, uvs+(divisions+1)*2);




  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &dataPtr);
  glGenBuffers(1, &indicePtr);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicePtr);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, dataPtr);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + texture_uvs.size() * sizeof(float), NULL, GL_STATIC_DRAW);

  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
  glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), texture_uvs.size() * sizeof(float), texture_uvs.data());

  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size() * sizeof(float)));

  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vTexture);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

}

RobotHead::~RobotHead(){
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &dataPtr);
  glGenBuffers(1, &indicePtr);
}

void RobotHead::init(AssetManager *am){
  texture = am->getTexture("r_head");
  shader = am->getShader("Default");
  if(shader!=nullptr){
    shader->bind();
    uModel = shader->getUniformLocation("model");
  }
}

void RobotHead::draw(float delta){
  if(shader!=nullptr){
    generateModelMatrix();
    shader->bind();
    shader->setMat4(uModel, model_matrix);
    shader->setBool("useTexture", true);
    if(texture!=nullptr){
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture->getID());
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicePtr);

    glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}
