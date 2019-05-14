#include "Robot.hpp"
#include "../AssetManager.hpp"
#include "../graphics/Texture.hpp"
Robot::Robot(){
  state = moving;
  vPosition = 0;
  animation_delta = 0;
  vTexture = 1;
  shader = nullptr;
  animation_step = 0;
  //Vertex positions
  float vp[] = {
    BODY_X/2.0f,  BODY_Y/2.0f, 0, // top right
    BODY_X/2.0f, -BODY_Y/2.0f, 0, // bottom right
    -BODY_X/2.0f, -BODY_Y/2.0f, 0, // bottom left
    -BODY_X/2.0f, BODY_Y/2.0f, 0 // top left
  };

  vertices = std::vector<float>(vp, vp+12);
  //Vertex indices
  unsigned int vi[] = { 0, 1, 3, 1, 2, 3 };
  indices = std::vector<unsigned int>(vi,vi+6);

  float uvs[] = { 1,1,1,0,0,0,0,1 };
  texture_uvs = std::vector<float>(uvs,uvs+8);


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

Robot::~Robot(){
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &dataPtr);
  glGenBuffers(1, &indicePtr);
}

void Robot::init(AssetManager *am){
  Animation a;
  a.push_back(am->getTexture("r_move1"));
  a.push_back(am->getTexture("r_move2"));

  animations.insert(std::make_pair(moving, a));
  a.clear();

  a.push_back(am->getTexture("r_idle"));
  animations.insert(std::make_pair(idle, a));
  animations.insert(std::make_pair(falling, a));

  shader = am->getShader("Default");
  if(shader!=nullptr){
    shader->bind();
    uModel = shader->getUniformLocation("model");
  }
  head.init(am);
  head.translate(glm::vec3(0,BODY_Y-11.5,0));
}

void Robot::draw(float delta){
  head.draw(delta);
  if(shader!=nullptr){
    generateModelMatrix();
    shader->bind();
    shader->setMat4(uModel, model_matrix);
    shader->setBool("useTexture", true);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, animations[state][animation_step]->getID());
    animation_delta+=delta;
    if(animation_delta > (animation_step+1)*(1.0f / animations[state].size())){
      animation_step++;
    }
    if(animation_delta>=1){
      animation_delta = 0;
    }
    if(animation_step>= animations[state].size())
      animation_step = 0;
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicePtr);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

}
