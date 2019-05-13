#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "../graphics/Scene.hpp"

class Robot;
#include "Terrain.hpp"
#include "Tile.hpp"
class ProjectScene : public Scene {
  private:
    Terrain terrain;
    Tile background;
  public:
    ProjectScene(Yuki* yuki);
    ~ProjectScene();


    void update(float delta);
    //Shader logic called every tick.
    void updateShaders();
    //Scene draw function.
    void draw(float delta);
    void resize(unsigned int w, unsigned int h);
    //Functions that will determine how our scene handles input.
    void keyPressedEventHandler(sf::Event::KeyEvent event);
    void keyStateEventHandler();
    void mouseButtonEventHandler(sf::Event::MouseButtonEvent event);
    void mouseMoveEventHandler(sf::Event::MouseMoveEvent event);

};

#endif
