#include "Project.hpp"
#include "../Yuki.hpp"
#include "../graphics/GraphicsEngine.hpp"
#include "../graphics/cameras/OrthoCamera.hpp"
#include "../AssetManager.hpp"
#include "../UserInput.hpp"

/**
\file Project.cpp
\brief Implementation of the Project class

\author Christopher Arausa
\version Final
\date 05/11/2019

*/

/**
\brief Constructor

This constructor sets up and initializes our scene with an orthographic camera, initializing terrain generation, the robot creation, everything. This is where it all began.
\param yuki --- Top level graphics engine class.
*/
ProjectScene::ProjectScene(Yuki* yuki) : Scene(yuki){
  //Our global ambient is a sort of blue hue
  global_ambient = glm::vec4(30.0/255.0, 44.0/255.0, 88.0/255.0, 1);
  //Setting up the camera
  int x = yuki->ge->getSize().x;
  int y = yuki->ge->getSize().y;
  addCamera("default", new OrthoCamera(x, y, 50));
  setActiveCamera("default");

  terrain.init(yuki->am);

  //Setting up our background tile.
  background.setShader(yuki->am->getShader("Default"));
  background.setPosition(glm::vec3(x/2.0,y/2.0,-1));
  background.setScale(glm::vec3(yuki->ge->getSize().x, yuki->ge->getSize().y,1));
  background.addTexture("background", yuki->am->getTexture("background"));
  background.setUseLighting(false);
  background.setActive(true);

  //Robot intialization
  robot.init(yuki->am);
  //Centering the robot and the camera on the center of the world quite high up so we don't clip into teh terrain.
  robot.move(glm::vec2(yuki->ge->getSize().x/2.0,yuki->ge->getSize().y/2.0));
  robot.move(glm::vec2(WORLD_WIDTH*PPM/2, WORLD_HEIGHT*PPM/2));
  getCamera()->setPosition(glm::vec3(WORLD_WIDTH*PPM/2, WORLD_HEIGHT*PPM/2,0));
  //Line width because beams are just lines right now.
  glLineWidth(10);
  beam = nullptr;
  beams_toggle = true;
}
/**
\brief Destructor

If we're using the beam, then it deletes it later.
*/
ProjectScene::~ProjectScene(){
  if(beam!=nullptr)
    delete beam;
}


/**
\brief This is the primary update logic of the scene.

All movement logic, animation state logic, and gravity is here.
\param delta --- The delta time since the last update.
*/
void ProjectScene::update(float delta){
  OrthoCamera* c = (OrthoCamera*)getCamera();
  //Starting positions of everything.
  glm::vec3 cpos = c->getPosition();
  glm::vec3 center_pos = robot.getPosition();
  glm::vec2 foot_pos = glm::vec2(center_pos.x, center_pos.y - 21);
  glm::vec2 head_pos = glm::vec2(center_pos.x, center_pos.y + 41);
  glm::vec2 body_r = glm::vec2(center_pos.x + 15, center_pos.y);
  glm::vec2 body_l = glm::vec2(center_pos.x - 15, center_pos.y);

  //Converted to world tile coordinates.
  sf::Vector2i foot_tile = getWorldPosition(glm::vec2(foot_pos.x, foot_pos.y));
  sf::Vector2i head_tile = getWorldPosition(glm::vec2(head_pos.x, head_pos.y));


  last_move+=delta;
  if(last_move>0.1f){
    robot.velocity.x = 0;
  }

  Tile below = terrain.getTile(foot_tile.x, foot_tile.y);
  //If we're not jumping and there is nothing below us, we're falling.
  if(!below.getActive() && robot.getState()!=jumping)
    robot.setState(falling);
  //If we're jumping or falling then apply gravity if we haven't hit terminal velocity.
  if((robot.getState()==falling || robot.getState()==jumping) && robot.velocity.y > (-max_vertical_velocity))
    robot.velocity.y -= gravity;
  //If we've peaked at our jump(upward velocity evening out), then we start falling
  if(robot.velocity.y < 0)
    robot.setState(falling);


  glm::vec2 new_center = glm::vec2(center_pos.x, center_pos.y) + (robot.velocity * PPM * delta);

  sf::Vector2i new_left_pos = getWorldPosition(glm::vec2(new_center.x - 15, new_center.y));
  sf::Vector2i new_right_pos = getWorldPosition(glm::vec2(new_center.x + 15, new_center.y));
  sf::Vector2i new_head_pos = getWorldPosition(glm::vec2(new_center.x, new_center.y+41));
  if(terrain.getTile(new_left_pos.x, new_left_pos.y).getActive() && robot.velocity.x < 0){
    robot.velocity.x = 0;
  }
  if(terrain.getTile(new_right_pos.x, new_right_pos.y).getActive() && robot.velocity.x > 0){
    robot.velocity.x = 0;
  }
  if(terrain.getTile(new_head_pos.x, new_head_pos.y).getActive() && robot.velocity.y > 0){
    robot.velocity.y = 0;
  }

  //Movement logic. We'll basically check to see where we are after the move, and if it violates any boundaries we compute the distance to the top of the tile.
  //And use that value instead.
  bool stop_drop = false;
  glm::vec2 new_foot = foot_pos + (robot.velocity * PPM * delta);
  sf::Vector2i new_foot_tile = getWorldPosition(glm::vec2(new_foot.x, new_foot.y));
  if(terrain.getTile(new_foot_tile.x, new_foot_tile.y).getActive()){
    //Let's calculate the needed translation to just land us on the top.
    float ydiff = new_foot_tile.y*32+16 - foot_pos.y;
    robot.velocity.y = ydiff;
    stop_drop=true;
  }

  robot.move(glm::vec2(robot.velocity*PPM*delta));
  c->translate(glm::vec3(robot.velocity.x*PPM*delta , robot.velocity.y*PPM*delta, 0));

  //State logic
  if(robot.getState()!=jumping && robot.velocity.x==0)
    robot.setState(idle);

  if(stop_drop){
    robot.setState(idle);
    robot.velocity.y = 0;
  }

  if((robot.getState()!=falling && robot.getState()!=jumping) && robot.velocity.x!=0 && robot.velocity.y ==0){
    robot.setState(moving);
  }
  else if(robot.velocity.y < 0)
    robot.setState(falling);

  if(yuki->ui->getMouseDown()){

  }
  else{
    if(beam!=nullptr){
      delete beam;
      beam = nullptr;
    }
  }
}


/**
\brief This function updates the shaders every tick.

It applies our global ambient and update the view/projection matrix.
*/
void ProjectScene::updateShaders(){
  Camera* camera = getCamera();
  for(auto const& [key, val] : yuki->am->getShaders()) {
    if(camera!=nullptr){
      camera->applyUpdate(val);
    }
    val->setVec4("global_ambient", global_ambient);
  }
}

/**
\brief This is our scene's draw function.

It draws the robot, the beam if it's active, the terrain, and clips the view matrix before drawing the background
\param delta --- float delta time since last draw.
*/
void ProjectScene::draw(float delta){
  robot.draw(delta);
  if(beam!=nullptr)
    beam->draw(delta);
 terrain.draw(delta);

   glm::mat4 viewMat = glm::mat4(glm::mat3(getCamera()->getView()));
   yuki->am->getShader("Default")->setMat4("view", viewMat);
   background.draw(delta);
}


/**
\brief Our event handler for when a key is pressed.
\param event --- sf::Event::KeyEvent event data.
*/
void ProjectScene::keyPressedEventHandler(sf::Event::KeyEvent event){
  if(event.code == sf::Keyboard::E){
   robot.toggleHeadlight();
  }
  if(event.code == sf::Keyboard::Q){
    beams_toggle = !beams_toggle;
  }
}

/**
\brief This function handles our key state logic.

This handles movement from the keyboard
*/
void ProjectScene::keyStateEventHandler(){
  glm::vec2 velocity(0);
  glm::vec3 robot_pos = robot.getPosition();
  sf::Vector2i robot_world_pos = getWorldPosition(glm::vec2(robot_pos.x, robot_pos.y-10));
  OrthoCamera* c = (OrthoCamera*)getCamera();

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
    if(robot.velocity.x > (-max_horizontal_velocity) && robot.getState()!=falling)
      velocity += glm::vec2(-horizontal_acceleration,0);
    if(robot.velocity.x > 0)
      robot.velocity.x = 0;
    last_move = 0;
  }

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
    if(robot.velocity.x<max_horizontal_velocity)
      velocity += glm::vec2(horizontal_acceleration,0);
    if(robot.velocity.x < 0)
      robot.velocity.x = 0;
    last_move = 0;
  }

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
    if(robot.velocity.y<max_vertical_velocity && terrain.getTile(robot_world_pos.x, robot_world_pos.y-1).getActive()  && robot.getState()!=falling){
      velocity += glm::vec2(0,jump_acceleration);
      robot.setState(jumping);
    }
    last_move = 0;
  }
  robot.velocity += velocity;
}
/**
\brief Our event handler for when a mouse button is pressed.

Right now this is only deleting tiles and firing beams.
\param event --- sf::Event::MouseButtonEvent event data.
*/
void ProjectScene::mouseButtonEventHandler(sf::Event::MouseButtonEvent event){
  glm::vec2 mpos = getMouseCoord();
  sf::Vector2i tile_loc = getWorldPosition(mpos);

  glm::vec2 robot_pos = glm::vec2(robot.head.getPosition().x, robot.head.getPosition().y);
  sf::Vector2i intersect = getFirstIntersection(robot_pos, mpos);

  if(event.button == sf::Mouse::Button::Left){
    if(beams_toggle && beam==nullptr){
      if(intersect.x!=-1)
        beam = new Beam(asset_manager, robot_pos, glm::vec2(intersect.x*32, intersect.y*32));
      else{
        beam = new Beam(asset_manager, robot_pos, mpos);
      }
    }
    if(intersect.x!=-1 && intersect.y!=-1){
      Tile t = terrain.getTile(intersect.x, intersect.y);
      if(t.getActive()){
        terrain.deleteTile(intersect.x, intersect.y);
      }
    }
  }
}

/**
\brief This function returns the mouse's current pixel location in the world

Return type glm::vec2
*/
glm::vec2 ProjectScene::getMouseCoord(){
  sf::Vector2i screen_coords = sf::Mouse::getPosition(*yuki->ge);
  glm::vec3 camera_pos = getCamera()->getPosition();
  glm::vec2 world_pos = glm::vec2(camera_pos.x + screen_coords.x, camera_pos.y + (yuki->ge->getSize().y - screen_coords.y));
  return world_pos;
}

/**
\brief This function converts world pixel location to world tilespace by dividing by the PPM and flooring by converting to integers.

Returns an sf::Vector2i with the converted value.
\param in --- glm::vec2 input value.
*/
sf::Vector2i ProjectScene::getWorldPosition(glm::vec2 in){
  sf::Vector2i tile_coord = sf::Vector2i((in.x+PPM/2.0)/PPM, int(in.y+PPM/2.0)/PPM);
  return tile_coord;
}
void ProjectScene::mouseMoveEventHandler(sf::Event::MouseMoveEvent event){
  UI *ui = yuki->ui;
  sf::Vector2f mouse_sensitivity = ui->getMouseSensitivity();

  robot.calcHeadRotation(getMouseCoord());
}
/**
\brief This function calls the getLineIntersectons method and trims down the input data to tilespace collisions

Returns a vector of sf::Vector2i containing all intersection positions.
\param start --- glm::vec2 starting position of the beam.
\param end --- glm::vec2 ending position of the beam.
*/
std::vector<sf::Vector2i> ProjectScene::getWorldIntersections(glm::vec2 start, glm::vec2 end){
  std::vector<glm::vec2> pixel_intersections = getLineIntersections(start, end);
  std::vector<sf::Vector2i> tile_intersections;
  std::map<std::pair<int,int>,bool> intersects;
  for(glm::vec2 pi : pixel_intersections){
    sf::Vector2i wpos = getWorldPosition(pi);
    std::pair<int,int> p = std::make_pair(wpos.x, wpos.y);
    if(intersects.count(p)==0){
      tile_intersections.push_back(wpos);
      intersects.insert(std::make_pair(p,true));
    }
  }
  return tile_intersections;
}
/**
\brief This function applies Bresenham's line algorithm to implement raycasting into the scene

Returns a vector of every vec2 tile in pixel coordinates that the line intersected.
Credits: https://playtechs.blogspot.com/2007/03/raytracing-on-grid.html
\param start --- vec2 starting vector
\param end --- vec2 ending vector
*/
std::vector<glm::vec2> ProjectScene::getLineIntersections(glm::vec2 start, glm::vec2 end){
  std::vector<glm::vec2> intersections;

  float dx = std::fabs(end.x - start.x);
  float dy = std::fabs(end.y - start.y);

  int x = int(std::floor(start.x));
  int y = int(std::floor(start.y));

  int n = 1;
  int x_inc, y_inc;

  double error;

  if(dx == 0){
    x_inc = 0;
    error = std::numeric_limits<double>::infinity();
  }
  else if(end.x > start.x){
    x_inc = 1;
    n+=int(std::floor(end.x))-x;
    error = (std::floor(start.x) + 1 - start.x)*dy;
  }
  else{
    x_inc = -1;
    n+=x - int(std::floor(end.x));
    error = (start.x - std::floor(start.x))*dy;
  }
  if(dy == 0){
    y_inc = 0;
    error -= std::numeric_limits<double>::infinity();
  }
  else if(end.y > start.y){
    y_inc = 1;
    n+= int(std::floor(end.y))-y;
    error -= (std::floor(start.y)+1-start.y)*dx;
  }
  else{
    y_inc = -1;
    n+=y - int(std::floor(end.y));
    error -= (start.y - std::floor(start.y))*dx;
  }
  for(; n>0; --n){
    intersections.push_back(glm::vec2(x,y));
    if(error > 0){
      y+=y_inc;
      error -= dx;
    }
    else{
      x+=x_inc;
      error += dy;
    }
  }
  return intersections;
}

/**
\brief This function takes in world pixel coordinates and returns the first intersection location clostest to the start.

\param start --- Starting position of the line.
\param end --- Ending position of the line.
*/
sf::Vector2i ProjectScene::getFirstIntersection(glm::vec2 start, glm::vec2 end){
  std::vector<sf::Vector2i> intersections = getWorldIntersections(start,end);
  sf::Vector2i best;
  float best_dist=-1;
  float curr_dist=1000000;
  for(sf::Vector2i i : intersections){
    Tile t = terrain.getTile(i.x, i.y);
    if(t.getActive()){
      curr_dist = std::sqrt(std::pow(int(start.x/32-(i.x)/32),2) + std::pow(int((start.y/32) - (i.y)/32),2));
      if(curr_dist < best_dist || best_dist == -1){
        best_dist = curr_dist;
        best = i;
      }
    }
  }
  if(best_dist == -1)
    return sf::Vector2i(-1,-1);
  else
    return best;
}

/**
\brief This function returns the first tile collision found the path between start and end.

Returns the tile that is collided or the empty_tile if nothing is found.
\param start --- Starting position of the line
\param end --- Ending position of the line.
*/
Tile ProjectScene::getFirstTile(glm::vec2 start, glm::vec2 end){
  sf::Vector2i intersect = getFirstIntersection(start,end);

  return terrain.getTile(intersect.x, intersect.y);
}
