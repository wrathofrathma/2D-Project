#include "Project.hpp"
#include "../Yuki.hpp"
#include "../graphics/GraphicsEngine.hpp"
#include "../graphics/cameras/OrthoCamera.hpp"
#include "../AssetManager.hpp"
#include "../UserInput.hpp"
ProjectScene::ProjectScene(Yuki* yuki) : Scene(yuki){
  global_ambient = glm::vec4(30.0/255.0, 44.0/255.0, 88.0/255.0, 1);
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

  if((robot.getState()!=falling && robot.getState()!=jumping) && robot.velocity.x!=0 && robot.velocity.y ==0){
    robot.setState(moving);
  }

  std::cout << "STATE " << robot.getState() << std::endl;
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
  glm::vec2 mpos = getMouseCoord();
  sf::Vector2i tile_loc = getWorldPosition(mpos);
  glm::vec2 robot_pos = glm::vec2(robot.head.getPosition().x, robot.head.getPosition().y);

  if(event.button == sf::Mouse::Button::Left){
    sf::Vector2i intersect = getFirstIntersection(robot_pos, mpos);
    if(intersect.x!=-1 && intersect.y!=-1){
      Tile t = terrain.getTile(intersect.x, intersect.y);
      if(t.getActive())
        terrain.deleteTile(intersect.x, intersect.y);
    }
  }
  getWorldIntersections(robot_pos, mpos);
}

glm::vec2 ProjectScene::getMouseCoord(){
  sf::Vector2i screen_coords = sf::Mouse::getPosition(*yuki->ge);
  glm::vec3 camera_pos = getCamera()->getPosition();
  glm::vec2 world_pos = glm::vec2(camera_pos.x + screen_coords.x, camera_pos.y + (yuki->ge->getSize().y - screen_coords.y));
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

Returns a vector of every vec2 tile in world coordinates that the line intersected.
\param start --- vec2 starting vector
\param end --- vec2 ending vector
*/
std::vector<glm::vec2> ProjectScene::getLineIntersections(glm::vec2 start, glm::vec2 end){
  std::vector<glm::vec2> intersections;
  float delta_x = end.x - start.x;
  float delta_y = std::abs(end.y - start.y);
  int x0 = start.x;
  int x1 = end.x;
  int y0 = start.y;
  int y1 = end.y;

  bool steep = std::abs(end.y - start.y) > std::abs(end.x - start.x);
  if(steep){
      int buf = x0;
      x0 = y0;
      y0 = buf;
  }
  if(x0 > x1){
    int buf = x0;
    x0 = x1;
    x1 = buf;
    buf = y0;
    y0 = y1;
    y1 = buf;
  }
  int e = 0;
  int ystep = 0;
  int y = y0;
  if(y0 < y1)
    ystep = 1;
  else
    ystep = -1;
  for(int x=x0; x<=x1; x++){
    if(steep)
      intersections.push_back(glm::vec2(y,x));
    else
      intersections.push_back(glm::vec2(x,y));
    e+=delta_y;
    if(2*e >= delta_x){
      y+= ystep;
      e-= delta_x;
    }
  }

  //
  // for(int x = start.x; x < end.x; x++){
  //   if(D > 0){
  //     cy++;
  //     delta-=2*dx;
  //     intersections.push_back(glm::vec2(int(x),int(cy)));
  //   }
  //   delta = delta+2*dy;
  // }

  return intersections;
}
sf::Vector2i ProjectScene::getFirstIntersection(glm::vec2 start, glm::vec2 end){
  std::vector<sf::Vector2i> intersections = getWorldIntersections(start,end);
  for(sf::Vector2i i : intersections){
    Tile t = terrain.getTile(i.x, i.y);
    if(t.getActive())
      return i;
  }
  return sf::Vector2i(-1,-1);
}

Tile ProjectScene::getFirstTile(glm::vec2 start, glm::vec2 end){
  sf::Vector2i intersect = getFirstIntersection(start,end);

  return terrain.getTile(intersect.x, intersect.y);
}
