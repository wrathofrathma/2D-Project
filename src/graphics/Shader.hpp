#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/type_ptr.hpp>

/**
\file Shader.hpp
\brief header file for Shader.cpp

\author Christopher Arausa
\version 0.1
\date 4/20/2019

*/


/**
\enum UNIFORM_ID
\brief Contains the location of common shader uniform values in our uniform array.
*/
enum UNIFORM_ID{
  MODEL,
  VIEW,
  PROJECTION,
};


/**
\class Shader

\brief The Shader class handles the loading, and toggling usage of a specific shader.

*/
class Shader {
  private:
    Shader(const Shader& other) {}
    void operator=(const Shader& other) {}
    static const unsigned int NUM_UNIFORMS = 3; ///< Our number of uniforms we're going to track. For now just the MVP matrices since I think we'll change textures a bit later.
    GLuint m_program; ///< Shader program ID.
    GLuint m_shaders[3]; ///< Array containing our shaders.
    GLuint m_uniforms[NUM_UNIFORMS]; ///< Array containing our uniform values so we don't have probe the graphics card every frame.
    GLuint createShader(const std::string &text, GLenum type);
    std::string getShaderString(GLenum);
    static std::string loadShader(const std::string& filename);
    bool checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
    bool loaded; ///< Boolean marking whether the shader has successfully loaded.
    std::string vfile; ///< Vertex shader filename.
    std::string ffile; ///< Fragment shader filename
    std::string gfile; ///< Geometry shader filename.
  public:
    Shader();
    Shader(const std::string &vert, const std::string &frag, const std::string &geom=std::string());
    ~Shader();
    GLint getID();
    void bind();
    bool isLoaded();
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, glm::vec3 &value);
    void setVec4(const std::string &name, glm::vec4 &value);
    void setMat4(const std::string &name, glm::mat4 &value);
    void setMat3(const std::string &name, glm::mat3 &value);

    void setBool(GLuint uniformLocation, bool value);
    void setInt(GLuint uniformLocation, int value);
    void setFloat(GLuint uniformLocation, float value);
    void setVec3(GLuint uniformLocation, glm::vec3 &value);
    void setVec4(GLuint uniformLocation, glm::vec4 &value);
    void setMat4(GLuint uniformLocation, glm::mat4 &value);
    void setMat3(GLuint uniformLocation, glm::mat3 &value);

    GLuint loadFromFile(const std::string &vert, const std::string &frag, const std::string &geom);
    void loadFromFile(std::string filename);
    GLuint getUniformLocation(const std::string &name);
    GLuint getUniformLocation(UNIFORM_ID type);
    std::string getFilenames();
};
#endif
