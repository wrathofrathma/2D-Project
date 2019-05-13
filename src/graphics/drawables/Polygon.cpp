#include "Polygon.hpp"
/**
\file Polygon.cpp
\brief Implementation of the Polygon class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/

/**
\brief Constructor

Calls the superclass drawable constructor and sets the default shader, orientation, and model matrix uniform.
*/
Polygon::Polygon(AssetManager *am) : Drawable(am){

  shader = am->getShader("Default");
  orientation = glm::vec3(0,0,0);
  setUseTexture(false);
  update = true;

  shader->bind();
  uModel = shader->getUniformLocation("model");
}

Polygon::Polygon(){
  orientation = glm::vec3(0,0,0);
  setUseTexture(false);
  update = true;
}
/**
\brief Destructor

Default empty destructor.
*/
Polygon::~Polygon(){

}
/**
\brief Returns the VAO of the polygon.

This should probably be moved to the drawable class.
*/
GLuint const Polygon::getVAO(){
  return VAO;
}
/**
\brief Sets the texture of the polygon by texture name.

Requests the texture from the asset manager and sets it if it doesn't come back null.
\param tex --- String id of the texture to request from the asset manager.
*/
void Polygon::setTexture(std::string tex){
  Texture* text = asset_manager->getTexture(tex);
  if(text!=nullptr)
    setTexture(text);
}

/**
\brief Returns the total vertex count of the polygon.

This might need to be changed in the future since I think we're storing vertices as floats, so it'd be3x the size. Also this should be moved probably.
*/
unsigned int Polygon::getVertexCount(){
  return vertices.size();
}

/**
\brief Sets the texture of the polygon to the texture pointer passed and generates texture_uvs.
\param tex --- Texture pointer of the texture to use.
*/
void Polygon::setTexture(Texture *tex){
  textures.push_back(tex);
  shader->bind();
  shader->setInt("texture1", 0);

  //Now we need to spread our texture
  for(unsigned int i=0; i<vertices.size(); i+=4){
    float v1 = vertices[i];
    float v2 = vertices[i+1];

    //Let's use these values, and assume our polygons are genrated at size 1, from 0.5 to -0.5 and scaled up using matrices later.
    //This way we can directly translate our textures early.
    texture_uvs.push_back(v1 + 0.5);
    texture_uvs.push_back(v2 + 0.5);
  }

  setUseTexture(true);
  update = true;
}

/**
\brief Sets the color of the polygon to one uniform color.
\param r --- Red component.
\param g --- Green component.
\param b --- Blue component.
*/
void Polygon::setColor(float r, float g, float b){
  colors.clear();
  for(unsigned int i=0; i<vertices.size()/4; i++){
    colors.push_back(r);
    colors.push_back(g);
    colors.push_back(b);
  }
  setUseTexture(false);
  update = true;
}
/**
\brief Sets the color of the polygon vertices.
\param c --- A vector containing colors for each vertex.
*/
void Polygon::setColor(std::vector<float> c){
  colors.clear();
  if(c.size() < 3)
    return;
  if(c.size() == vertices.size())
    colors = c;
  else {
    //Here we have more vertices than colors or more colors, let's treat it as one color.
    for(unsigned int i=0; i<vertices.size()/4; i++){
      colors.push_back(c[0]);
      colors.push_back(c[1]);
      colors.push_back(c[2]);
    }
  }
  setUseTexture(false);
  update = true;
}

/**
\brief Draws the polygon.
*/
void Polygon::draw(){
  initDraw();
  if(useTexture){
    if(textures.size()>0){
      glActiveTexture(GL_TEXTURE0); //Activate texture unit
      glBindTexture(GL_TEXTURE_2D, textures[0]->getID());
    }
  }
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicePtr);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
