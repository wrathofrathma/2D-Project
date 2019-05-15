#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "../graphics/Scene.hpp"

#include "Terrain.hpp"
#include "Tile.hpp"
#include "Robot.hpp"
#include "Beam.hpp"

/**
\mainpage 2D Project

\tableofcontents

\section intro Introduction

There is a randomly generated world that is constructed from a heightmap I built for the 3D project. I locked the heightmap to a single dimension and used that to generate heights.
The terrain itself is filled in randomly using some logic in the Terrain class, defining what blocks can spawn where. It works for the most part, but it's not perfect(If you look close you'll notice the flesh and stone values are swapped.)
Using a system like this for terrain generation in a 2D world is significantly simpler than its 3D counterpart and could probably get some rather intersting worlds if you were to add more tiles.

There user controls a robot that traverses this 2D world. He is equipped with some rather basic animations, and movement logic, collision detection, and a laser.
Surrounding the robot is a nearby radius of light that's created in the shader, as well as a conal headlight spawning from the robot's eye.

The beams themselves are implemented using simple GL_LINES right now and are toggled on and off with Q. The beams follow the path to the first collision site.

If the user clicks on a tile, a line is drawn from the eye of the robot to the mouse position using Bresenham's line algorithm that I found online.

There is an orthographic camera that follows the user around keeping the player centered in the view. (Although resizing kind of fucks this up and I was more interested in playing with beams than to fix that. Jokes on me, I didn't fix either lmao)

The background is kept in place by clipping the transformation portion of the view matrix before rendering the background.

I trimmed down this submission, so most of the relevant code is in the scenes/ folder but I think everything included is used except for materials.

\subsection options User Options
- Escape: Ends the program
- M: Toggles between fill and line mods to draw triangles.
- F10: Takes a screenshot.
- A: Moves the robot left
- D: Moves the robot right.
- E: Toggles the robot's headlight.
- Q: Toggles beams
- Space: The robot jumps.

If the user moves the mouse, the robot's eye will track the mouse keeping its light centered on it.
If the user left clicks on a tile, the nearest tile in that line will be deleted.
*/

/**
\file Project.hpp
\brief Header file for Project.cpp and primary header for the project.

\author Christopher Arausa
\version Final
\date 05/14/2019
*/

/**
\class ProjectScene
\brief This class is the top level logic class of the program. All logic is contained here.
*/
class ProjectScene : public Scene {
  private:
    Terrain terrain; ///< Terrain class to generate and house our world.
    Tile background; ///< The background is just a tile scaled up with its view matrix clipped.
    Robot robot; ///< The user or robot class.
    static constexpr float max_horizontal_velocity = 5; ///< Maximum horizontal velocity
    static constexpr float max_vertical_velocity = 20; ///< Maximum vertical velocity
    static constexpr float horizontal_acceleration =0.5; ///< The acceleration applied when the user strafes.
    static constexpr float jump_acceleration = 10; ///< The acceleration applied when the user jumps.
    static constexpr float gravity = 2; ///< The gravity constant of the world.
    static constexpr float PPM = 32; ///< Pixels per unit/meter. It's a divisor used to translate from opengl coordinates to tile/world coordinates.

    Beam* beam; ///< The beam that I was playing with.
    glm::vec4 radial_lighting; ///< The radial lighting value that surrounds the player.
    float last_move; ///< The time since the player last moved. I think I used to track idle status with this.
    bool beams_toggle; ///< The flag that enables beams.
  public:
    ProjectScene(Yuki* yuki);
    ~ProjectScene();


    void update(float delta);
    //Shader logic called every tick.
    void updateShaders();
    //Scene draw function.
    void draw(float delta);
    //Functions that will determine how our scene handles input.
    void keyPressedEventHandler(sf::Event::KeyEvent event);
    void keyStateEventHandler();
    void mouseButtonEventHandler(sf::Event::MouseButtonEvent event);
    void mouseMoveEventHandler(sf::Event::MouseMoveEvent event);

    glm::vec2 getMouseCoord();
    sf::Vector2i getWorldPosition(glm::vec2);
    std::vector<glm::vec2> getLineIntersections(glm::vec2 start, glm::vec2 end);
    std::vector<sf::Vector2i> getWorldIntersections(glm::vec2 start, glm::vec2 end);
    Tile getFirstTile(glm::vec2 start, glm::vec2 end);
    sf::Vector2i getFirstIntersection(glm::vec2 start, glm::vec2 end);

};

#endif
