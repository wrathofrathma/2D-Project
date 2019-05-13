#include "Project.hpp"
#include "../Yuki.hpp"
#include "../graphics/GraphicsEngine.hpp"
#include "../graphics/cameras/OrthoCamera.hpp"
#include "../AssetManager.hpp"
ProjectScene::ProjectScene(Yuki* yuki) : Scene(yuki){
  int x = yuki->ge->getSize().x;
  int y = yuki->ge->getSize().y;
  addCamera("default", new OrthoCamera(x, y, 50));
  setActiveCamera("default");

  terrain.init(yuki->am);
  background.setShader(yuki->am->getShader("Default"));
  //background.scale(glm::vec3(200,200,1));
  background.setPosition(glm::vec3(x/2.0,y/2.0,-1));
  background.setScale(glm::vec3(yuki->ge->getSize().x, yuki->ge->getSize().y,1));
  background.addTexture("background", yuki->am->getTexture("background"));

}

ProjectScene::~ProjectScene(){

}


void ProjectScene::update(float delta){

}

//Shader logic called every tick.
void ProjectScene::updateShaders(){
  Camera* camera = getCamera();
  for(auto const& [key, val] : yuki->am->getShaders()) {
    if(camera!=nullptr){
      camera->applyUpdate(val);
    }
  }
}

//Scene draw function.
void ProjectScene::draw(float delta){
  terrain.draw(delta);

  glm::mat4 viewMat = glm::mat4(glm::mat3(getCamera()->getView()));
  yuki->am->getShader("Default")->setMat4("view", viewMat);
  background.draw(delta);

}

void ProjectScene::resize(unsigned int w, unsigned int h){

}

//Functions that will determine how our scene handles input.
void ProjectScene::keyPressedEventHandler(sf::Event::KeyEvent event){
  if(event.code == sf::Keyboard::Up){

  }
}

void ProjectScene::keyStateEventHandler(){

}

void ProjectScene::mouseButtonEventHandler(sf::Event::MouseButtonEvent event){

}

void ProjectScene::mouseMoveEventHandler(sf::Event::MouseMoveEvent event){

}
