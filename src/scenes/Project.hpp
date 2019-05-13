#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "../graphics/Scene.hpp"
class ProjectScene : public Scene {
  private:

  public:
    ProjectScene(Yuki* yuki);
    ~ProjectScene();


    void update(float delta);
    //Shader logic called every tick.
    void updateShaders();
    //Scene draw function.
    void draw();
    void resize(unsigned int w, unsigned int h);
    //Functions that will determine how our scene handles input.
    void keyPressedEventHandler(sf::Event::KeyEvent event);
    void keyStateEventHandler();
    void mouseButtonEventHandler(sf::Event::MouseButtonEvent event);
    void mouseMoveEventHandler(sf::Event::MouseMoveEvent event);

};

#endif
