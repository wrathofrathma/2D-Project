#include "Scene.hpp"
#include "../Yuki.hpp"
#include "cameras/Camera.hpp"
#include "components/Light.hpp"
#include "components/Drawable.hpp"
#include"../AssetManager.hpp"

/**
\file Scene.cpp
\brief Implementation of the Scene class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/

/**
\brief Constructor

Default constructor that just stores our game engine pointer.
\param yuki --- Yuki class pointer.
*/
Scene::Scene(Yuki* yuki) {
  this->yuki = yuki;
  this->asset_manager = yuki->am;
  global_ambient = glm::vec4(0.1);
}

/**
\brief Destructor

Virtual destructor that cleans up any objects, cameras, and lighting in the scene.
*/
Scene::~Scene(){
  clearCameras();
}

/**
\brief Calculates the delta time between updates, updates the camera matrices, and then calls virtual function update() which contains scene logic.

This is called every iteration in the primary game loop.
*/
void Scene::tick(){
  //Generate delta
  sf::Time curr_time = clock.getElapsedTime();
  sf::Time delta = curr_time - last_time;
  float dt = delta.asSeconds();
  last_time = curr_time;
  //Generate camera updates
  Camera *c = getCamera();
  if(c!=nullptr)
    c->update();
  updateShaders();
  update(dt);
}

/**
\brief Adds a camera to the scene.

If a camera of the id already exists, it overwrites that camera.
\param id --- Key to store the camera in our map under.
\param c --- Camera pointer of the camera to store.
*/
void Scene::addCamera(std::string id, Camera* c){
  if(cameras.count(id)>0){
    std::cerr << "Camera ID already exists. Overwriting old camera" << std::endl;
    removeCamera(id);
  }
  cameras.insert(std::pair<std::string,Camera*>(id,c));
}
/**
\brief Removes a camera from the scene.
\param id --- Camera ID to remove.
*/
void Scene::removeCamera(std::string id){
  if(cameras.size()==0)
		return;
	if(cameras.count(id)==0)
		return;
	//The actual deletion
  if(cameras[id]!=nullptr){
	 delete cameras[id];
   cameras[id] = nullptr;
  }
	cameras.erase(id);
	//If our deleted camera was our active camera, then we should pick the camera at the bottom of the map. If one doesn't exist then we just set to "".
	if(active_camera.compare(id)==0){
		if(cameras.size()>0)
			active_camera = cameras.begin()->first;
		else
			active_camera.clear();
	}
}

/**
\brief Removes all cameras.
*/
void Scene::clearCameras(){
  while(cameras.size()>0){
    std::map<std::string, Camera*>::iterator it = cameras.begin();
    if(it->second!=nullptr){
      delete it->second;
      it->second = nullptr;
    }
    cameras.erase(it);
  }
  cameras.clear();
  active_camera.clear();
}


/**
\brief Sets the active camera in the scene.
\param id --- ID of the camera to set as active.
*/
void Scene::setActiveCamera(std::string id){
  if(cameras.count(id)==0){
    std::cerr << "Couldn't set active camera. ID not found." << std::endl;
    return;
  }
  active_camera = id;
}
/**
\brief Returns the active camera of the scene.
*/
Camera* Scene::getCamera(){
  if(cameras.size()==0 || active_camera.size()==0 || cameras.count(active_camera)==0){
    std::cerr << "Couldn't find active camera." << std::endl;
    return nullptr;
  }
	return cameras[active_camera];
}
/**
\brief Applies resize updates to the projection matrix by updating the cameras.
*/
void Scene::resize(unsigned int w, unsigned int h){
  for(auto const& [key, val] : cameras) {
    if(val!=nullptr)
      val->resize(w,h);
  }
}
/**
\brief Sets the global ambient lighting value of the scene.
*/
void Scene::setGlobalAmbient(glm::vec4 a){
  global_ambient = a;
}
/**
\brief Returns the global ambient lighting value of the scene.
*/
glm::vec4 Scene::getGlobalAmbient(){
  return global_ambient;
}

/**
\brief Virtual function for scene key event logic.
*/
void Scene::keyPressedEventHandler(sf::Event::KeyEvent event){

}
/**
\brief Virtual function for scene key state logic.
*/
void Scene::keyStateEventHandler(){

}
/**
\brief Virtual function for scene mouse button logic.
*/
void Scene::mouseButtonEventHandler(sf::Event::MouseButtonEvent event){

}
/**
\brief Virtual function for scene mouse move logic.
*/
void Scene::mouseMoveEventHandler(sf::Event::MouseMoveEvent event){

}
