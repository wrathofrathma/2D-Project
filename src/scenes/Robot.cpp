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
  headlight = true;
  float vp[] = {
    BODY_X/2.0f,  BODY_Y/2.0f, 0, // top right
    BODY_X/2.0f, -BODY_Y/2.0f, 0, // bottom right
    -BODY_X/2.0f, -BODY_Y/2.0f, 0, // bottom left
    -BODY_X/2.0f, BODY_Y/2.0f, 0 // top left
  };
  velocity = glm::vec2(0);
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
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &dataPtr);
  glDeleteBuffers(1, &indicePtr);
}

void Robot::toggleHeadlight(){
  headlight = !headlight;
}
void Robot::init(AssetManager *am){
  Animation a;
  a.push_back(am->getTexture("r_move1"));
  a.push_back(am->getTexture("r_move2"));
  a.push_back(am->getTexture("r_move3"));
  animations.insert(std::make_pair(moving, a));
  a.clear();

  a.push_back(am->getTexture("r_idle1"));
  a.push_back(am->getTexture("r_idle2"));
  a.push_back(am->getTexture("r_idle3"));
  animations.insert(std::make_pair(idle, a));
  a.clear();

  a.push_back(am->getTexture("r_jump1"));
  a.push_back(am->getTexture("r_jump2"));
  a.push_back(am->getTexture("r_jump3"));
  animations.insert(std::make_pair(jumping, a));
  a.clear();

  a.push_back(am->getTexture("r_fall1"));
  a.push_back(am->getTexture("r_fall2"));
  a.push_back(am->getTexture("r_fall3"));
  animations.insert(std::make_pair(falling, a));

  shader = am->getShader("Default");
  if(shader!=nullptr){
    shader->bind();
    uModel = shader->getUniformLocation("model");
  }
  head.init(am);
  head.translate(glm::vec3(0,BODY_Y-11.5,0));
}

void Robot::move(glm::vec2 value){
  translate(glm::vec3(value.x, value.y, 0),false);
  head.translate(glm::vec3(value.x, value.y, 0),false);
}

void Robot::setState(ROBOT_STATE s){
  state = s;
}
ROBOT_STATE Robot::getState(){
  return state;
}
void Robot::draw(float delta){
  head.draw(delta);
  if(shader!=nullptr){
    generateModelMatrix();
    shader->bind();
    shader->setMat4(uModel, model_matrix);
    shader->setBool("use_texture", true);
    shader->setBool("use_lighting", true);
    shader->setBool("use_headlight", headlight);
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

void Robot::calcHeadRotation(glm::vec2 value){
  //We need to calculate the directional vector between the head center and the mouse.
  glm::vec3 head_pos = head.getPosition();
  double angle = atan2(value.y - head_pos.y, value.x - head_pos.x);

  head.setOrientation(glm::vec3(0,0,angle));
}
