#include "Material.hpp"
/**
\file Material.cpp
\brief Implementation of the Material class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/

/**
\brief Constructor

Default constructor. Initializes the ambient, diffuse, specular, emissive, and shininess values.
*/
Material::Material(){
  setAmbient(glm::vec4(1,1,1,1));
  setDiffuse(glm::vec4(1,1,1,1));
  setSpecular(glm::vec4(0.1,0.1,0.1,1));
  setEmission(glm::vec4(0.1));
  setShininess(0);
}
/**
\brief Constructor

Sets the ambient, diffuse, specular, emissive, and shininess values.
\param a --- Ambient lighting value.
\param d --- Diffuse lighting value.
\param s --- Specular lighting value.
\param e --- Emissive lighting value.
\param sh --- Shininess of the material.
*/
Material::Material(glm::vec4 a, glm::vec4 d, glm::vec4 s, glm::vec4 e, float sh){
  ambient = a;
  diffuse = d;
  specular = s;
  emission = e;
  shininess = sh;
}
/**
\brief Copy constructor.
*/
Material::Material(Material &m1){
  setAmbient(m1.getAmbient());
  setDiffuse(m1.getDiffuse());
  setSpecular(m1.getSpecular());
  setEmission(m1.getEmission());
  setShininess(m1.getShininess());
}
/**
\brief Destructor

Default destructor with nothing to clean up.
*/
Material::~Material(){

}
/**
\brief Returns the ambient color of the material.
*/
glm::vec4 Material::getAmbient(){
  return ambient;
}
/**
\brief Returns the diffuse color of the material.
*/
glm::vec4 Material::getDiffuse(){
  return diffuse;
}
/**
\brief Returns the specular color of the material.
*/
glm::vec4 Material::getSpecular(){
  return specular;
}
/**
\brief Returns the emissive color of the material.
*/
glm::vec4 Material::getEmission(){
  return emission;
}
/**
\brief Returns the shininess of the material.
*/
float Material::getShininess(){
  return shininess;
}
/**
\brief Sets the ambient color of the material.
\param a --- Ambient value of the material.
*/
void Material::setAmbient(glm::vec4 a){
  ambient = a;
}
/**
\brief Sets the diffuse color of the material.
\param d --- Diffuse value of the material.
*/
void Material::setDiffuse(glm::vec4 d){
  diffuse = d;
}
/**
\brief Sets the specular color of the material.
\param s --- Specular value of the material.
*/
void Material::setSpecular(glm::vec4 s){
  specular = s;
}
/**
\brief Sets the emissive color of the material.
\param e --- Emission color of the material.
*/
void Material::setEmission(glm::vec4 e){
  emission = e;
}
/**
\brief Sets the shininess of the material.
\param sh --- Shininess of the material.
*/
void Material::setShininess(float sh){
  shininess = sh;
}
