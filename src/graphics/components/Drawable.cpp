#include "Drawable.hpp"
/**
\file Drawable.cpp
\brief Implementation of the Drawable class.

\author Christopher Arausa
\version 0.1 Alpha
\date 4/20/2019

*/
/**
\brief Constructor

Sets default shader position IDs and calls init() to generate buffers.
*/
Drawable::Drawable(){
  vPosition = 0;
  vNormal = 1;
  vColor = 2;
  vTexture = 3;
  lighting_on = true;
}

void Drawable::setAssetManager(AssetManager *am){
  asset_manager = am;
  init();
}

/**
\brief Constructor

Sets default shader position IDs and calls init() to generate buffers.
\param am --- Asset manager.
*/
Drawable::Drawable(AssetManager *asset_manager){
  this->asset_manager = asset_manager;
  vPosition = 0;
  vNormal = 1;
  vColor = 2;
  vTexture = 3;
  lighting_on = true;
  init();
}
/**
\brief Destructor

Virtual method that defaults to calling cleanup() to release VAO/EBO/VBO data on teh graphics card.
*/
Drawable::~Drawable(){
  cleanup();
}
/**
\brief Initializes our object by generating vertex arrays and buffers.
*/
void Drawable::init(){
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &dataPtr);
  glGenBuffers(1, &indicePtr);
}
/**
\brief Returns the material being used by the drawable.
*/
Material& Drawable::getMaterial(){
  return material;
}


/**
\brief Releases VAO/VBO/EBO data on the graphics card.
*/
void Drawable::cleanup(){
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &dataPtr);
  glDeleteBuffers(1, &indicePtr);
}
/**
\brief Tells the class to bind a texture on the draw loop and sets the uniform on our shader for if we're using a texture.
\param use --- The boolean value to set for if we're using a texture.
*/
void Drawable::setUseTexture(bool use){
  shader->bind();
  shader->setBool("useTexture", use);
  useTexture = use;
}
/**
\brief Returns the associated asset manager.
*/
AssetManager *Drawable::getAssetManager(){
  return asset_manager;
}
/**
\brief Sets our vertice data for the object and marks our update flag.
\param v --- Vertice data vector to use.
*/
void Drawable::setVertices(std::vector<float> v){
  vertices = v;
  update = true;
}
/**
\brief Sets our indice data for the object and marks our update flag.
\param ind --- Indice data vector to use.
*/
void Drawable::setIndices(std::vector<unsigned int> ind){
  indices = ind;
  update = true;
}
/**
\brief Sets our texture uv data for the object and marks our update flag.
\param uvs --- Texture UV data vector to use.
*/
void Drawable::setTextureUVs(std::vector<float> uvs){
  texture_uvs = uvs;
  update = true;
}
/**
\brief Sets our normal data for the object and marks our update flag.
\param norms --- Normal data vector to use.
*/
void Drawable::setNormals(std::vector<float> norms){
  normals = norms;
  update = true;
}
/**
\brief Sets our color data for the object and marks our update flag.
\param c --- Color data vector to use.
*/
void Drawable::setColors(std::vector<float> c){
  colors = c;
  update = true;
}
/**
\brief Sets our vertice data for the object and marks our update flag.
\param v --- Vertex array data to use.
\param count --- Number of elements in the array.
*/
void Drawable::setVertices(float* v, unsigned int count){
  vertices.clear();
  vertices = std::vector<float>(v, v+count);
  update = true;
}
/**
\brief Sets our indice data for the object and marks our update flag.
\param ind --- Indice array data to use.
\param count --- Number of elements in the array.
*/
void Drawable::setIndices(unsigned int* ind, unsigned int count){
  indices.clear();
  indices = std::vector<unsigned int>(ind, ind+count);
  update = true;
}
/**
\brief Sets our texture uv data for the object and marks our update flag.
\param uvs --- Texture UV array data to use.
\param count --- Number of elements in the array.
*/
void Drawable::setTextureUVs(float* uvs, unsigned int count){
  texture_uvs.clear();
  texture_uvs = std::vector<float>(uvs, uvs+count);
  update = true;
}
/**
\brief Sets our normal data for the object and marks our update flag.
\param norms --- Normal array data to use.
\param count --- Number of elements in the array.
*/
void Drawable::setNormals(float* norms, unsigned int count){
  normals.clear();
  normals = std::vector<float>(norms, norms+count);
  update = true;
}
/**
\brief Sets our color data for the object and marks our update flag.
\param c --- Color array data to use.
\param count --- Number of elements in the array.
*/
void Drawable::setColors(float* c, unsigned int count){
  colors.clear();
  colors = std::vector<float>(c, c+count);
  update = true;
}

/**
\brief Adds textures to our texture vector.
\param texts --- Vector of textures to add.
*/
void Drawable::addTexture(std::vector<Texture*> texts){
  for(Texture *t : texts)
    textures.push_back(t);
}
/**
\brief Adds a single texture to our texture vector.
\param text --- Texture to add.
*/
void Drawable::addTexture(Texture* text){
    textures.push_back(text);
}
/**
\brief Loads all vertice, color, texture, indice, and normal data to the graphics card.

This is a virtual function that is overloaded in more complex shapes.
*/
void Drawable::updateGraphicsCard(){
  unsigned int vertice_size = vertices.size() * sizeof(float);
  unsigned int color_size = colors.size() * sizeof(float);
  unsigned int text_size = texture_uvs.size() * sizeof(float);
  unsigned int indice_size = indices.size() * sizeof(unsigned int);
  unsigned int normal_size = normals.size() * sizeof(float);
  unsigned int data_size = text_size + color_size + vertice_size + normal_size;

  glBindVertexArray(VAO);
  //Load indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicePtr);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_size, &indices[0], GL_DYNAMIC_DRAW);

  //Allocate space for data
  glBindBuffer(GL_ARRAY_BUFFER, dataPtr);
  glBufferData(GL_ARRAY_BUFFER, data_size, NULL, GL_DYNAMIC_DRAW);

  //Load data as parts of sub partitions of the buffer
  //vertices
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertice_size, &vertices[0]);
  //Color
  glBufferSubData(GL_ARRAY_BUFFER, vertice_size, color_size, &colors[0]);
  //Texture
  glBufferSubData(GL_ARRAY_BUFFER, vertice_size + color_size, text_size, &texture_uvs[0]);
  //Normals
  glBufferSubData(GL_ARRAY_BUFFER, vertice_size + color_size + text_size, normal_size, &normals[0]);
  //Tell the shader how to find its data
  glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertice_size + color_size));
  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(vertice_size));
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertice_size + color_size + text_size));

  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vColor);
  glEnableVertexAttribArray(vNormal);
  glEnableVertexAttribArray(vTexture);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  update = false;
}

/**
\brief Sets the material type of our object.

This is kind of primitive and I'm not sure if we'll be able to use this for more complex shapes.
\param mat --- Material to use.
*/
void Drawable::setMaterial(Material mat){
  material = mat;
  update = true;
}
/**
\brief Sets the shader to use by name.
\param name --- String of the shader id to ask the asset manager for.
*/
void Drawable::setShader(const std::string &name){
  cleanup();
  shader = asset_manager->getShader(name);
  update = true;
  shader->bind();
  uModel = shader->getUniformLocation("model");
  init();
}
/**
\brief Sets the shader to use directly by setting the shader.
\param s --- Pointer to the shader to use.
*/
void Drawable::setShader(Shader *s){
  cleanup();
  shader = s;
  update = true;
  shader->bind();
  uModel = shader->getUniformLocation("model");
  init();
}

/**
\brief Loads the material data to the shader.
*/
void Drawable::loadMaterial(){
  shader->bind();
  glm::vec4 amb = material.getAmbient();
  glm::vec4 diff = material.getDiffuse();
  glm::vec4 spec = material.getSpecular();
  glm::vec4 emis = material.getEmission();
  shader->setVec4("material.ambient",amb);
  shader->setVec4("material.diffuse", diff);
  shader->setVec4("material.specular",spec);
  shader->setFloat("material.shininess",material.getShininess());
  shader->setVec4("material.emission",emis);
}
/**
\brief A pre-draw initialization that many primitives use to a void a long draw function definition.

Binds the shader, Generates and updates the model matrix, tells the shader if we're using a texture, loads the material, and then calls updateGraphicsCard() if needed.
*/
void Drawable::initDraw(){
  shader->bind();
  generateModelMatrix();
  shader->setMat4(uModel, model_matrix);
  shader->setBool("lighting_on", lighting_on);
  setUseTexture(useTexture);
  loadMaterial(); //Let's change this later when when we start loading proper meshes. Maybe then we can move texturing to material class.
  if(update){
    updateGraphicsCard();
  }
  //Also let's send a normal matrix.
  glm::mat3 normal_matrix = glm::mat3(glm::transpose(glm::inverse(model_matrix)));
  shader->setMat3("normal_matrix", normal_matrix);
}
/**
\brief Sets whether to apply lighting to this object.
\param onoff --- Boolean value to use.
*/
void Drawable::setLightingOn(bool onoff){
  lighting_on = onoff;
}
/**
\brief Returns whether to apply lighting to this object.
*/
bool Drawable::getLightingOn(){
  return lighting_on;
}
