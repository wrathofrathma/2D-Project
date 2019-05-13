#include "GraphicsDefinitions.hpp"

/**
\file GraphicsDefinitions.cpp
\brief Implementation functions in the GraphicsDefintion.cpp file.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/

/**
\brief Prints all OpenGL errors to stderr.

*/

using namespace std;
void printOpenGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        fprintf(stderr, "OpenGL Error: %s\n", errString);
    }
}

/**
\brief Utility function to calculate surface normals on basic polygons.

Returns a vector containing the normal of the surface.
\param vertices --- Vector containing all of the vertices on the polygon.
*/
glm::vec3 calcSurfaceNormal(std::vector<glm::vec3> vertices){
  glm::vec3 normal = glm::vec3(0.0f);

  glm::vec3 current = glm::vec3(0.0f);
  glm::vec3 next = glm::vec3(0.0f);
  for(unsigned int i = 0; i < vertices.size() - 1; i++){
    current = vertices[i];
    next = vertices[i+1];
    normal.x+= (current.y - next.y) * (current.z + next.z);
    normal.y+= (current.z - next.z) * (current.x + next.x);
    normal.z+= (current.x - next.x) * (current.y + next.y);
  }
  return glm::normalize(normal);
}
