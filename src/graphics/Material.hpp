#ifndef YUKI_MATERIAL_HPP
#define YUKI_MATERIAL_HPP
#include <glm/glm.hpp>
/**
\file Material.hpp
\brief Header file for Material.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 5/8/2019

*/

/**
\class Material
\brief The material class represents how an object reacts to lighting.
*/
class Material {
  private:
    glm::vec4 ambient; ///< Ambient color of the material.
    glm::vec4 diffuse; ///< Diffuse color of the material.
    glm::vec4 specular; ///< Specular color of the material.
    glm::vec4 emission; ///< Emission color of the material - Not yet implemented.
    float shininess; ///< Shininess of the material. 
  public:
    Material(glm::vec4 a, glm::vec4 d, glm::vec4 s, glm::vec4 e, float shininess);
    Material();
    Material(Material &m1);
    ~Material();

    glm::vec4 getAmbient();
    glm::vec4 getDiffuse();
    glm::vec4 getSpecular();
    glm::vec4 getEmission();
    float getShininess();
    void setAmbient(glm::vec4 a);
    void setDiffuse(glm::vec4 d);
    void setSpecular(glm::vec4 s);
    void setEmission(glm::vec4 e);
    void setShininess(float sh);
};

#endif
