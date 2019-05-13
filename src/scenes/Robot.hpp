#ifndef ROBOT_HPP
#define ROBOT_HPP

class AssetManager;
class Quad;
class Robot {
  private:
    AssetManager *am;
    static constexpr float body_radius = 16;
    static constexpr float body_height = 60;

    Quad* body;
    Quad* head;
    void loadTextures();
  public:
    Robot(AssetManager* am);
    void draw();
};

#endif
