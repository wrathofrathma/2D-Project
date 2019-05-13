#ifndef GRAPHICS_DEFINITIONS_HPP
#define GRAPHICS_DEFINITIONS_HPP
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "Material.hpp"

/**
\file GraphicsDefinitions.hpp
\brief Basic material properties for normal surfaces and some other miscelleneous definitions.

\author Christopher Arausa
\version 0.1
\date 4/27/2019
*/

#define BUFFER_OFFSET(x) ((const void*) (x))
#define PI 3.14159265358979323846264338328
#define PI_DIV_180 0.0174532925199432957692369076849
#define deg PI_DIV_180

void printOpenGLErrors();
glm::vec3 calcSurfaceNormal(std::vector<glm::vec3> vertices);

/**
\namespace Materials

\brief The Materials namespace is simply a collection of statically
defined material attributes.
- Material::Default --- Default
- Material::greenTint --- Green tint.
- Material::redPlastic --- Red Plastic
- Material::greenPlastic --- Green Plastic
- Material::bluePlastic --- Blue Plastic
- Material::brass --- Brass
- Material::bronze --- Bronze
- Material::polishedBronze --- Polished Bronze
- Material::chrome --- Chrome
- Material::copper --- Copper
- Material::polishedCopper --- Polished Copper
- Material::gold --- Gold
- Material::polishedGold --- Polished Gold
- Material::pewter --- Pewter
- Material::silver --- Silver
- Material::polishedSilver --- Polished Silver
- Material::emerald --- Emerald
- Material::jade --- Jade
- Material::obsidian --- Obsidian
- Material::pearl --- Pearl
- Material::ruby --- Ruby
- Material::turquoise --- Turquoise

*/

namespace Materials
{

  static Material greenTint(
    glm::vec4(0.0, 0.1, 0.0, 1.0),
    glm::vec4(0.0, 0.1, 0.0, 1.0),
    glm::vec4(0.1, 0.1, 0.1, 1.0),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    32.0
  );

  static Material Default(
      glm::vec4(0.4),
      glm::vec4(0.4),
      glm::vec4(0.4),
      glm::vec4(0.4),
      32.0
  );
  static Material Dark(
      glm::vec4(0.1),
      glm::vec4(0.1),
      glm::vec4(0.1),
      glm::vec4(0.1),
      32.0
  );

static Material redPlastic(
    glm::vec4(0.3, 0.0, 0.0, 1.0),
    glm::vec4(0.6, 0.0, 0.0, 1.0),
    glm::vec4(0.8, 0.6, 0.6, 1.0),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    32.0
);

static Material greenPlastic(
    glm::vec4(0.0, 0.3, 0.0, 1.0),
    glm::vec4(0.0, 0.6, 0.0, 1.0),
    glm::vec4(0.6, 0.8, 0.6, 1.0),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    32.0
);

static Material bluePlastic(
    glm::vec4(0.0, 0.0, 0.3, 1.0),
    glm::vec4(0.0, 0.0, 0.6, 1.0),
    glm::vec4(0.6, 0.6, 0.8, 1.0),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    32.0
);

static Material brass(
    glm::vec4(0.329412, 0.223529, 0.027451, 1),
    glm::vec4(0.780392, 0.568627, 0.113725, 1),
    glm::vec4(0.992157, 0.941176, 0.807843, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    27.8974
);

static Material bronze(
    glm::vec4(0.2125, 0.1275, 0.054, 1),
    glm::vec4(0.714, 0.4284, 0.18144, 1),
    glm::vec4(0.393548, 0.271906, 0.166721, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    25.6
);

static Material polishedBronze(
    glm::vec4(0.25, 0.148, 0.06475, 1),
    glm::vec4(0.4, 0.2368, 0.1036, 1),
    glm::vec4(0.774597, 0.458561, 0.200621, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    76.8
);

static Material chrome(
    glm::vec4(0.25, 0.25, 0.25, 1),
    glm::vec4(0.4, 0.4, 0.4, 1),
    glm::vec4(0.774597, 0.774597, 0.774597, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    76.8
);

static Material copper(
    glm::vec4(0.19125, 0.0735, 0.0225, 1),
    glm::vec4(0.7038, 0.27048, 0.0828, 1),
    glm::vec4(0.256777, 0.137622, 0.086014, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    12.8
);

static Material polishedCopper(
    glm::vec4(0.2295, 0.08825, 0.0275, 1),
    glm::vec4(0.5508, 0.2118, 0.066, 1),
    glm::vec4(0.580594, 0.223257, 0.0695701, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    51.2
);

static Material gold(
    glm::vec4(0.24725, 0.1995, 0.0745, 1),
    glm::vec4(0.75164, 0.60648, 0.22648, 1),
    glm::vec4(0.628281, 0.555802, 0.366065, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    51.2
);

static Material polishedGold(
    glm::vec4(0.24725, 0.2245, 0.0645, 1),
    glm::vec4(0.34615, 0.3143, 0.0903, 1),
    glm::vec4(0.797357, 0.723991, 0.208006, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    83.2
);

static Material pewter(
    glm::vec4(0.105882, 0.058824, 0.113725, 1),
    glm::vec4(0.427451, 0.470588, 0.541176, 1),
    glm::vec4(0.333333, 0.333333, 0.521569, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    9.84615
);

static Material silver(
    glm::vec4(0.19225, 0.19225, 0.19225, 1),
    glm::vec4(0.50754, 0.50754, 0.50754, 1),
    glm::vec4(0.508273, 0.508273, 0.508273, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    51.2
);

static Material polishedSilver(
    glm::vec4(0.23125, 0.23125, 0.23125, 1),
    glm::vec4(0.2775, 0.2775, 0.2775, 1),
    glm::vec4(0.773911, 0.773911, 0.773911, 1),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    89.6
);

static Material emerald(
    glm::vec4(0.0215, 0.1745, 0.0215, 0.55),
    glm::vec4(0.07568, 0.61424, 0.07568, 0.55),
    glm::vec4(0.633, 0.727811, 0.633, 0.55),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    76.8
);

static Material jade(
    glm::vec4(0.135, 0.2225, 0.1575, 0.95),
    glm::vec4(0.54, 0.89, 0.63, 0.95),
    glm::vec4(0.316228, 0.316228, 0.316228, 0.95),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    12.8
);

static Material obsidian(
    glm::vec4(0.05375, 0.05, 0.06625, 0.82),
    glm::vec4(0.18275, 0.17, 0.22525, 0.82),
    glm::vec4(0.332741, 0.328634, 0.346435, 0.82),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    38.4
);

static Material pearl(
    glm::vec4(0.25, 0.20725, 0.20725, 0.922),
    glm::vec4(1, 0.829, 0.829, 0.922),
    glm::vec4(0.296648, 0.296648, 0.296648, 0.922),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    11.264
);

static Material ruby(
    glm::vec4(0.1745, 0.01175, 0.01175, 0.55),
    glm::vec4(0.61424, 0.04136, 0.04136, 0.55),
    glm::vec4(0.727811, 0.626959, 0.626959, 0.55),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    76.8
);

static Material turquoise(
    glm::vec4(0.1, 0.18725, 0.1745, 0.8),
    glm::vec4(0.396, 0.74151, 0.69102, 0.8),
    glm::vec4(0.297254, 0.30829, 0.306678, 0.8),
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    12.8
);

}
#endif
