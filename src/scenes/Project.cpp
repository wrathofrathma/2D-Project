#include "Project.hpp"
#include "../Yuki.hpp"
#include "../graphics/GraphicsEngine.hpp"
#include "../graphics/cameras/OrthoCamera.hpp"
#include "../AssetManager.hpp"
#include "../UserInput.hpp"
ProjectScene::ProjectScene(Yuki* yuki) : Scene(yuki){
  global_ambient = glm::vec4(40.0/255.0, 54.0/255.0, 98.0/255.0, 1);
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
  background.setUseLighting(false);
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




  //std::cout << t.getID() << std::endl;
  last_move+=delta;
  if(last_move>0.1f){
    robot.velocity.x = 0;
  }

  Tile below = terrain.getTile(foot_tile.x, foot_tile.y);

  //Acceleration section. If there is no block immediately stopping us, we proceed.
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


}

//Shader logic called every tick.
void ProjectScene::updateShaders(){
  Camera* camera = getCamera();
  for(auto const& [key, val] : yuki->am->getShaders()) {
    if(camera!=nullptr){
      camera->applyUpdate(val);
    }
    val->setVec4("global_ambient", global_ambient);
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
  sf::Vector2i robot_world_pos = getWorldPosition(glm::vec2(robot_pos.x, robot_pos.y-10));
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
  if(event.button == sf::Mouse::Button::Left){
    glm::vec2 mpos = getMouseCoord();
    sf::Vector2i tile_loc = getWorldPosition(mpos);
    terrain.deleteTile(tile_loc.x, tile_loc.y);
  }
}

glm::vec2 ProjectScene::getMouseCoord(){
  sf::Vector2i screen_coords = sf::Mouse::getPosition(*yuki->ge);
  glm::vec3 camera_pos = getCamera()->getPosition();
  glm::vec2 world_pos = glm::vec2(camera_pos.x + screen_coords.x, camera_pos.y + (yuki->ge->getSize().y - screen_coords.y));
  std::cout << "Mouse to World: " << world_pos.x << " " << world_pos.y << std::endl;
  std::cout << "Tile: " << getWorldPosition(glm::vec2(world_pos)).x << " " <<getWorldPosition(glm::vec2(world_pos)).y << std::endl;
  return world_pos;
}

sf::Vector2i ProjectScene::getWorldPosition(glm::vec2 in){
  sf::Vector2i tile_coord = sf::Vector2i((in.x+PPM/2.0)/PPM, int(in.y+PPM/2.0)/PPM);
  return tile_coord;
}
void ProjectScene::mouseMoveEventHandler(sf::Event::MouseMoveEvent event){
  UI *ui = yuki->ui;
  sf::Vector2f mouse_sensitivity = ui->getMouseSensitivity();

  robot.calcHeadRotation(getMouseCoord());
}
