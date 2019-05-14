#include "Robot.hpp"
#include "../AssetManager.hpp"
#include "../graphics/Texture.hpp"
Robot::Robot(AssetManager* am){
  if(am!=nullptr)
    init(am);
}

Robot::~Robot(){

}

void Robot::init(AssetManager *am){
  this->am = am;
  Animation a;
  a.push_back(am->getTexture("r_move1"));
  a.push_back(am->getTexture("r_move1"));

  animations.insert(std::make_pair(moving, a));
  a.clear();

  a.push_back(am->getTexture("r_idle"));
  animations.insert(std::make_pair(idle, a));
  head.init(am);
}

void Robot::draw(float delta){
  head.draw(delta);
}
