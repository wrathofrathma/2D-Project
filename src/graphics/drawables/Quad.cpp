#include "Quad.hpp"
/**
\file Quad.cpp
\brief Implementation of the Quad class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/

/**
\brief Constructor

Sets the dimensions of our quad with default values of a unit square.
\param am --- AssetManager class pointer.
\param x --- Top length of the quad.
\param y --- Right length of the quad.
\param z --- Bottom length of the quad.
\param w --- Left length of the quad.
*/
Quad::Quad(AssetManager *am, float x, float y, float z, float w) : Polygon(am){
  setDimensions(glm::vec4(x,y,z,w));

}
Quad::Quad(){

}
void Quad::loadQuad(AssetManager *am, float x, float y, float z, float w){
  asset_manager = am;
  setDimensions(glm::vec4(x,y,z,w));

}
/**
\brief Destructor

Default empty destructor.
*/
Quad::~Quad(){

}

/**
\brief Generates our quad's vertices, normals, and sets a default color of (1,1,1).
*/
void Quad::generateQuad(){
  //Vertices
  float vv[] = {
	     dimensions.x/2.0f,  dimensions.y/2.0f, 0.0f, 1.0f, // top right
	     dimensions.z/2.0f, -dimensions.y/2.0f, 0.0f, 1.0f, // bottom right
	     -dimensions.z/2.0f, -dimensions.w/2.0f, 0.0f, 1.0f, // bottom left
	     -dimensions.x/2.0f, dimensions.w/2.0f, 0.0f,  1.0f  // top left
	};

  //Normals
  std::vector<glm::vec3> vvn;
  vvn.push_back(glm::vec3(dimensions.x/2.0f,  dimensions.y/2.0f, 0.0f));
  vvn.push_back(glm::vec3(dimensions.z/2.0f, -dimensions.y/2.0f, 0.0f));
  vvn.push_back(glm::vec3(-dimensions.z/2.0f, -dimensions.w/2.0f, 0.0f));
  vvn.push_back(glm::vec3(-dimensions.x/2.0f, dimensions.w/2.0f, 0.0f));
  glm::vec3 norms = calcSurfaceNormal(vvn);

  float vn[] = {
    norms.x, norms.y, norms.z,
    norms.x, norms.y, norms.z,
    norms.x, norms.y, norms.z,
    norms.x, norms.y, norms.z
  };
  setNormals(vn, 12);
  //Indices
  unsigned int vi[] = { 0, 1, 3, 1, 2, 3 };

  //Load using drawable inherited functions.
  setVertices(vv, 16);
  setIndices(vi, 6);
  //Set to white as a default texture.
  setColor(1,1,1);
}
/**
\brief Sets the dimensions of the quad.
\param dims --- glm::vec4 containing the length of each side of the quad.
*/
void Quad::setDimensions(glm::vec4 dims){
  dimensions = dims;
  generateQuad();
}
