#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "../graphics/Scene.hpp"

#include "Terrain.hpp"
#include "Tile.hpp"
#include "Robot.hpp"
class ProjectScene : public Scene {
  private:
    Terrain terrain;
    Tile background;
    Robot robot;
    static constexpr float max_horizontal_velocity = 5;
    static constexpr float max_vertical_velocity = 20;
    static constexpr float horizontal_acceleration =0.5;
    static constexpr float jump_acceleration = 10;
    static constexpr float gravity = 5;
    static constexpr float PPM = 32;


    glm::vec4 radial_lighting;
    float last_move;
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

    glm::vec2 getMouseCoord();
    sf::Vector2i getWorldPosition(glm::vec2);
};

#endif
