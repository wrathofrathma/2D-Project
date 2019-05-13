#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <glm/vec4.hpp> // glm::vec4
#include <SFML/System.hpp>
class Yuki;
class Camera;
class Light;
class Drawable;
class AssetManager;

/**
\file Scene.hpp
\brief header file for Scene.cpp

\author Christopher Arausa
\version 0.1
\date 5/7/2019

*/

/**
\class AssetManager

\brief The scene class is a top level container to hold all relevant objects and functions relative to a scene or area.
Things such as UI input handling methods, drawable objects, lighting objects, update logic threads. Etc.
*/

class Scene {
  protected:
    Yuki* yuki; ///< Our link back up to the rest of our resources.
    AssetManager* asset_manager; ///< Having a direct link to our asset manager is handy.
    //Camera stuff
    std::map<std::string, Camera*> cameras; ///< Map containing any cameras associated with this scene.
    std::string active_camera; ///< String containing the active camera in the scene.

    //Update stuff
    sf::Time last_time; ///< Time variable containing our last measured time for delta calculations.
    sf::Clock clock; ///< Clock for determining delta value between updates.

    glm::vec4 global_ambient; ///< A vec4 containing our global ambient for the scene. It'll be used in our lighting updates later.
    void clearCameras();

  public:
    Scene(Yuki* yuki);
    virtual ~Scene();
    void tick(); //Handles pre-update events such as calcing delta time and updating camera matrices.
    //Scene logic function.
    virtual void update(float delta)=0;
    //Shader logic called every tick.
    virtual void updateShaders()=0;
    //Scene draw function.
    virtual void draw()=0;
    virtual void resize(unsigned int w, unsigned int h);
    //Functions that will determine how our scene handles input.
    virtual void keyPressedEventHandler(sf::Event::KeyEvent event);
    virtual void keyStateEventHandler();
    virtual void mouseButtonEventHandler(sf::Event::MouseButtonEvent event);
    virtual void mouseMoveEventHandler(sf::Event::MouseMoveEvent event);

    void setGlobalAmbient(glm::vec4 a);
    glm::vec4 getGlobalAmbient();
    AssetManager* getAssetManager(){return asset_manager;}
    //Camera management.
    void setActiveCamera(std::string s);
    void addCamera(std::string s, Camera* c);
    void removeCamera(std::string s);
		Camera* getCamera();

};

#endif
