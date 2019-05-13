#include "Project.hpp"
#include "../Yuki.hpp"
#include "../graphics/GraphicsEngine.hpp"
#include "../graphics/cameras/FreeCamera.hpp"
ProjectScene::ProjectScene(Yuki* yuki) : Scene(yuki){
  addCamera("default", new FreeCamera(yuki->ge->getSize().x, yuki->ge->getSize().y, 50));
  setActiveCamera("default");
}

ProjectScene::~ProjectScene(){

}


void ProjectScene::update(float delta){

}

//Shader logic called every tick.
void ProjectScene::updateShaders(){

}

//Scene draw function.
void ProjectScene::draw(){

}

void ProjectScene::resize(unsigned int w, unsigned int h){

}

//Functions that will determine how our scene handles input.
void ProjectScene::keyPressedEventHandler(sf::Event::KeyEvent event){

}

void ProjectScene::keyStateEventHandler(){

}

void ProjectScene::mouseButtonEventHandler(sf::Event::MouseButtonEvent event){

}

void ProjectScene::mouseMoveEventHandler(sf::Event::MouseMoveEvent event){

}
