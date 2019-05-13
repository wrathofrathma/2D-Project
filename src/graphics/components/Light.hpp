#ifndef YUKI_LIGHT_HPP
#define YUKI_LIGHT_HPP

#include "SpacialObject.hpp"
#include "../Shader.hpp"
/**
\file Light.hpp
\brief Header file for Light.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/8/2019

*/

/**
\enum LIGHT_TYPE
\brief Enum describing whether the light is point, directional, or a spot light.
*/
enum LIGHT_TYPE {
  POINT,
  DIRECTIONAL,
  SPOT
};

/**
\class Light
\brief Component class for anything that emits light in the program.
*/

class Light : virtual public SpacialObject {
  protected:
    bool on; ///< Light on or off.
    glm::vec4 ambient; ///< Ambient color of the light.
    glm::vec4 diffuse; ///< Diffuse color of the light.
    glm::vec4 specular; ///< Specular color of the light.
    float constant; ///< Attenuation constant value
    float linear; ///< Attenuation linear value
    float quadratic; ///< Attenuation quadratic value
    float cutOff; ///< Spotlight cutoff value
    float outerCutOff; ///< Spotlight outer cutoff value.
    LIGHT_TYPE type; ///< The type of light, point, directional, or spot light.
  public:

    Light();
    //Directional constructor
    Light(bool onoff, glm::vec4 dir, glm::vec4 a, glm::vec4 d, glm::vec4 s);
    //Point constructor
    Light(bool onoff, glm::vec4 pos, glm::vec4 a, glm::vec4 d, glm::vec4 s, float c, float l, float q);
    //Generic
    Light(LIGHT_TYPE t, bool onoff, glm::vec4 a, glm::vec4 d, glm::vec4 s);
    Light(LIGHT_TYPE t);
    ~Light();

    bool getOn();
    glm::vec4 getAmbient();
    glm::vec4 getDiffuse();
    glm::vec4 getSpecular();
    float getConstant();
    float getLinear();
    float getQuadratic();
    LIGHT_TYPE getType();
    void setType(LIGHT_TYPE t);
    void setConstant(float c);
    void setLinear(float l);
    void setQuadratic(float q);
    void setOn(bool onoff);
    void setAmbient(glm::vec4 a);
    void setDiffuse(glm::vec4 d);
    void setSpecular(glm::vec4 s);
    void setCutOff(float c);
    void setOuterCutOff(float oc);
    float getCutOff();
    float getOuterCutOff();
    static void setOff(Shader *s, unsigned int i, LIGHT_TYPE type);
    void loadToShader(Shader *s, unsigned int i);
};

#endif
