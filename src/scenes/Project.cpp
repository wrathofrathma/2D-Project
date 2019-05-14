#include "Project.hpp"
#include "../Yuki.hpp"
#include "../graphics/GraphicsEngine.hpp"
#include "../graphics/cameras/OrthoCamera.hpp"
#include "../AssetManager.hpp"
#include "../UserInput.hpp"
ProjectScene::ProjectScene(Yuki* yuki) : Scene(yuki){
  int x = yuki->ge->getSize().x;
  int y = yuki->ge->getSize().y;
  addCamera("default", new OrthoCamera(x, y, 50));
  setActiveCamera("default");
  // getCamera()->setPosition(glm::vec3(64,30,1));
  terrain.init(yuki->am);
  background.setShader(yuki->am->getShader("Default"));
  background.setPosition(glm::vec3(x/2.0,y/2.0,-1));
  background.setScale(glm::vec3(yuki->ge->getSize().x, yuki->ge->getSize().y,1));
  background.addTexture("background", yuki->am->getTexture("background"));
  background.setActive(true);
  robot.init(yuki->am);
  robot.move(glm::vec2(yuki->ge->getSize().x/2.0,yuki->ge->getSize().y/2.0));
  robot.move(glm::vec2(WORLD_WIDTH*PPM/2, WORLD_HEIGHT*PPM/2));
  getCamera()->setPosition(glm::vec3(WORLD_WIDTH*PPM/2, WORLD_HEIGHT*PPM/2,0));
}

ProjectScene::~ProjectScene(){

}


void ProjectScene::update(float delta){
  OrthoCamera* c = (OrthoCamera*)getCamera();
  glm::vec3 robo_pos = robot.getPosition();
  sf::Vector2i tile_coord = getWorldPosition(glm::vec2(robo_pos.x, robo_pos.y));
  last_move+=delta;
  if(last_move>0.2f){
    robot.velocity.x = 0;
  }
  std::cout << "Velocity: " << robot.velocity.x << " " << robot.velocity.y << std::endl;
  bool moved = false;
  Tile below = terrain.getTile(tile_coord.x, tile_coord.y-1);
  if(!below.getActive() && robot.getState()!=falling){
    robot.setState(falling);
    robot.velocity.y-=gravity;
  }
  //Movement logic.
  if(robot.velocity.y > 0){
    //Chekc stuff above head height and apply gravity.
    Tile above = terrain.getTile(tile_coord.x, tile_coord.y+1);
    if(above.getActive()){
      robot.velocity.y = 0;
      robot.setState(falling);
    }

  }
  else if(robot.velocity.y < 0){
    if(below.getActive() && robot.getState()!=jumping){
      robot.velocity = glm::vec2(robot.velocity.x, 0);
      robot.setState(idle);
    }
  }

  if(robot.velocity.x > 0){
    moved = true;
    //Moving right.
    Tile right = terrain.getTile(tile_coord.x+1, tile_coord.y);
    Tile right_upper = terrain.getTile(tile_coord.x+1, tile_coord.y+1);
    if(right_upper.getActive()){
      //If there is a tile at head level to teh right, our velocity stops
      robot.velocity = glm::vec2(0,robot.velocity.y);
    }
    else if(right.getActive()){
      //if there is no tile to the upper right, but one to our right, then we need to apply an upwards velocity if one doesn't exist.
      if(robot.velocity.y==0){
        robot.move(glm::vec2(0,PPM));
        c->translate(glm::vec3(0,PPM,0));
      }
    }
  }
  else if(robot.velocity.x < 0){
    bool moved = false;
    Tile left = terrain.getTile(tile_coord.x-1, tile_coord.y);
    Tile left_upper = terrain.getTile(tile_coord.x-1, tile_coord.y+1);
    if(left_upper.getActive()){
      robot.velocity.x = 0;
    }
    else if(left.getActive()){
      if(robot.velocity.y==0){
        robot.move(glm::vec2(0,PPM));
        c->translate(glm::vec3(0,PPM,0));
      }
    }
  }
  if(moved && robot.getState()==idle){
    robot.setState(moving);
  }
  else if(!moved && robot.getState()==moving)
    robot.setState(idle);

  robot.move(glm::vec2(robot.velocity));
  c->translate(glm::vec3(robot.velocity.x*PPM*delta , robot.velocity.y*PPM*delta, 0));


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
  robot.draw(delta);

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
  glm::vec2 velocity(0);
  glm::vec3 robot_pos = robot.getPosition();
  sf::Vector2i robot_world_pos = getWorldPosition(glm::vec2(robot_pos.x, robot_pos.y));
  OrthoCamera* c = (OrthoCamera*)getCamera();
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){

  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
    if(robot.velocity.x > (-max_horizontal_velocity) && robot.getState()!=falling)
      velocity += glm::vec2(-horizontal_acceleration,0);
    if(robot.velocity.x > 0)
      robot.velocity.x = 0;
    last_move = 0;
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){

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

void ProjectScene::mouseButtonEventHandler(sf::Event::MouseButtonEvent event){
}

glm::vec2 ProjectScene::getMouseCoord(){
  sf::Vector2i screen_coords = sf::Mouse::getPosition(*yuki->ge);
  glm::vec3 camera_pos = getCamera()->getPosition();
  glm::vec2 world_pos = glm::vec2(camera_pos.x + screen_coords.x, camera_pos.y + (yuki->ge->getSize().y - screen_coords.y));
  std::cout << world_pos.x << " " << world_pos.y << std::endl;
  return world_pos;
}

sf::Vector2i ProjectScene::getWorldPosition(glm::vec2 in){
  sf::Vector2i tile_coord = sf::Vector2i(int(in.x+PPM)/PPM, int(in.y+PPM)/PPM);
  return tile_coord;
}
void ProjectScene::mouseMoveEventHandler(sf::Event::MouseMoveEvent event){
  UI *ui = yuki->ui;
  sf::Vector2f mouse_sensitivity = ui->getMouseSensitivity();

  robot.calcHeadRotation(getMouseCoord());
}
