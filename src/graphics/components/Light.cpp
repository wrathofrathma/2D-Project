#include "Light.hpp"
/**
\file Light.cpp
\brief Implementation of the Light class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/

/**
\brief Constructor

Sets the default attenuation values, and uses a color of (0.5, 0.5, 0.5) for default ambient/specular/diffuse colors.
Also initializes the position to 0 and defaults to a Point light.
*/
Light::Light(){
  setType(POINT);
  setOn(true);
  setPosition(glm::vec3(0));
  orientation = glm::vec3(0);
  setAmbient(glm::vec4(0.5));
  setDiffuse(glm::vec4(0.5));
  setSpecular(glm::vec4(0.5));
  constant = 1.0f;
  linear = 0;
  quadratic = 0;
  linear = 0.045f;
  quadratic = 0.0075f;
}
/**
\brief Constructor

Sets the light type to the passed value and initializes default values.
Sets the default attenuation values, and uses a color of (0.5, 0.5, 0.5) for default ambient/specular/diffuse colors.
\param t --- Type of light.
*/
Light::Light(LIGHT_TYPE t){
  setType(t);
  setOn(true);
  setPosition(glm::vec3(0));
  orientation = glm::vec3(0);
  setAmbient(glm::vec4(0.5));
  setDiffuse(glm::vec4(0.5));
  setSpecular(glm::vec4(0.5));
  constant = 1.0f;
  linear = 0.045f;
  quadratic = 0.0075f;
}
/**
\brief Constructor

Directional constructor.
\param onoff --- Is the light on or off?
\param dir --- Direction the light is coming from.
\param a --- Ambient value of the light.
\param d --- Diffuse value of the light.
\param s --- Specular value of the light.
*/
Light::Light(bool onoff, glm::vec4 dir, glm::vec4 a, glm::vec4 d, glm::vec4 s){
  setType(DIRECTIONAL);
  setOn(onoff);
  orientation = dir;
  setAmbient(a);
  setDiffuse(d);
  setSpecular(s);
}
/**
\brief Constructor

Point constructor.
\param onoff --- Is the light on or off?
\param pos --- Position of the light.
\param a --- Ambient value of the light.
\param d --- Diffuse value of the light.
\param s --- Specular value of the light.
\param c --- Attenuation constant.
\param l --- Attenuation linear.
\param q --- Attenuation quadratic.
*/
Light::Light(bool onoff, glm::vec4 pos, glm::vec4 a, glm::vec4 d, glm::vec4 s, float c, float l, float q){
  setType(POINT);
  setOn(onoff);
  setPosition(pos);
  setAmbient(a);
  setDiffuse(d);
  setSpecular(s);
  setConstant(c);
  setLinear(l);
  setQuadratic(q);
}

/**
\brief Constructor

Generic constructor that sets defaults based on the light type passed.
\param t --- Type of light.
\param onoff --- Is the light on or off?
\param a --- Ambient value of the light.
\param d --- Diffuse value of the light.
\param s --- Specular value of the light.
*/
Light::Light(LIGHT_TYPE t, bool onoff, glm::vec4 a, glm::vec4 d, glm::vec4 s){
  setType(t);
  setOn(onoff);
  setAmbient(a);
  setDiffuse(d);
  setSpecular(s);
  if(t == POINT){
    constant = 1.0f;
    linear = 0.045f;
    quadratic = 0.0075f;
  }
  orientation = (glm::vec3(0));
}

/**
\brief Destructor

Generic destructor, nothing to cleanup.
*/
Light::~Light(){

}

/**
\brief Returns if the light is on or off.
*/
bool Light::getOn(){
  return on;
}
/**
\brief Returns the ambient value of the light.
*/
glm::vec4 Light::getAmbient(){
  return ambient;
}
/**
\brief Returns the diffuse value of the light.
*/
glm::vec4 Light::getDiffuse(){
  return diffuse;
}
/**
\brief Returns the specular value of the light.
*/
glm::vec4 Light::getSpecular(){
  return specular;
}
/**
\brief Sets if the light is on or off.
\param onoff -- Boolean value for if the light is on.
*/
void Light::setOn(bool onoff){
  on = onoff;
}
/**
\brief Sets the light's ambient value.
\param a --- glm::vec4 containing the new ambient value of the light.
*/
void Light::setAmbient(glm::vec4 a){
  ambient = a;
}
/**
\brief Sets the light's diffuse value.
\param d --- glm::vec4 containing the new diffuse value of the light.
*/
void Light::setDiffuse(glm::vec4 d){
  diffuse = d;
}
/**
\brief Sets the light's specular value.
\param s --- glm::vec4 containing the new specular value of the light.
*/
void Light::setSpecular(glm::vec4 s){
  specular = s;
}

/**
\brief Static function that tells the shader to turn off lights that we're not using or don't exist.
\param s --- Pointer to the shader to update.
\param i --- Index of the shader array we're updating.
\param type --- Type of light we're turning off.
*/
void Light::setOff(Shader *s, unsigned int i, LIGHT_TYPE type){
  s->bind();
  std::string struct_pos="";
  switch(type){
    case POINT: {
      struct_pos+="pointLights[";
      struct_pos+=std::to_string(i);
      struct_pos+="].";
      s->setBool((struct_pos + "on"), false);
      }
      break;
    case DIRECTIONAL: {
      struct_pos+="directionalLights[";
      struct_pos+=std::to_string(i);
      struct_pos+="].";
      s->setBool((struct_pos + "on"), false);
      }
      break;
    case SPOT: {
      struct_pos+="spotLights[";
      struct_pos+=std::to_string(i);
      struct_pos+="].";
      s->setBool((struct_pos + "on"), false);
      }
      break;
  };
}
/**
\brief Loads the light's data to the shader.
\param s --- Shader to give our data to.
\param i --- Index of the shader's light array we belong to.
*/
void Light::loadToShader(Shader *s, unsigned int i){
  s->bind();
  std::string struct_pos="";
  switch(type){
    case POINT: {
      struct_pos+="pointLights[";
      struct_pos+=std::to_string(i);
      struct_pos+="].";
      glm::vec4 pos(position, 1.0);
      s->setBool((struct_pos + "on"), on);
      s->setVec4((struct_pos + "position"),pos);
      s->setVec4((struct_pos + "ambient"), ambient);
      s->setVec4((struct_pos + "diffuse"), diffuse);
      s->setVec4((struct_pos + "specular"), specular);
      s->setFloat((struct_pos + "constant"), constant);
      s->setFloat((struct_pos + "linear"), linear);
      s->setFloat((struct_pos + "quadratic"), quadratic);
      }
      break;
    case DIRECTIONAL: {
      struct_pos+="directionalLights[";
      struct_pos+=std::to_string(i);
      struct_pos+="].";
      s->setBool((struct_pos + "on"), on);
      glm::vec4 dir(orientation,1);
      s->setVec4((struct_pos + "direction"), dir);
      s->setVec4((struct_pos + "ambient"), ambient);
      s->setVec4((struct_pos + "diffuse"), diffuse);
      s->setVec4((struct_pos + "specular"), specular);
      }
      break;
    case SPOT: {
      struct_pos+="spotLights[";
      struct_pos+=std::to_string(i);
      struct_pos+="].";
      glm::vec4 pos(position, 1.0);
      s->setBool((struct_pos + "on"), on);
      s->setVec4((struct_pos + "position"),pos);
      s->setVec4((struct_pos + "ambient"), ambient);
      s->setVec4((struct_pos + "diffuse"), diffuse);
      s->setVec4((struct_pos + "specular"), specular);
      s->setFloat((struct_pos + "constant"), constant);
      s->setFloat((struct_pos + "linear"), linear);
      s->setFloat((struct_pos + "quadratic"), quadratic);
      s->setFloat((struct_pos + "cutOff"), cutOff);
      s->setFloat((struct_pos + "outerCutOff"), outerCutOff);
      }
      break;
  };
}
/**
\brief Returns the light's attenuation constant value.
*/
float Light::getConstant(){
  return constant;
}
/**
\brief Returns the light's attenuation linear value.
*/
float Light::getLinear(){
  return linear;
}
/**
\brief Returns the light's attenuation quadratic value.
*/
float Light::getQuadratic(){
  return quadratic;
}
/**
\brief Returns the light's type.
*/
LIGHT_TYPE Light::getType(){
  return type;
}
/**
\brief Sets the light's type.
\param t --- LIGHT_TYPE describing whether we're point, directional, or a spot light.
*/
void Light::setType(LIGHT_TYPE t){
  type = t;
}
/**
\brief Sets the light's attenuation constant value.
\param c --- Constant value to use.
*/
void Light::setConstant(float c){
  constant = c;
}
/**
\brief Sets the light's attenuation linear value.
\param l --- Linear value to use.
*/
void Light::setLinear(float l){
  linear = l;
}
/**
\brief Sets the light's attenuation quadratic value.
\param q --- Quadratic value to use.
*/
void Light::setQuadratic(float q){
  quadratic = q;
}
/**
\brief Sets the light's spot light cutoff value.
\param c --- Cutoff value to use.
*/
void Light::setCutOff(float c){
  cutOff = c;
}
/**
\brief Sets the light's spot light outer cutoff value.
\param oc --- Outer cutoff value to use.
*/
void Light::setOuterCutOff(float oc){
  outerCutOff = oc;
}
/**
\brief Returns the light's current cutoff value.
*/
float Light::getCutOff(){
  return cutOff;
}
/**
\brief Returns the light's current outer cutoff value.
*/
float Light::getOuterCutOff(){
  return outerCutOff;
}
