#version 420 core


out vec4 FragColor; ///< Our resulting color.

uniform float time;

void main()
{
  float r = clamp(1.0-(0.1)*time, 0.0, 1.0);
  float g = 0.2;
  float b = clamp(0.05*time, 0.0, 1.0);
  FragColor = vec4(r,g,b,1);
}
