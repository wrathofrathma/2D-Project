#version 420 core



in vec4 frag_pos; ///< Fragment position
in vec2 tex_coord; ///< Texture coordinates.

out vec4 FragColor; ///< Our resulting color.

uniform sampler2D texture1; ///< Texture to use if we're using one.
uniform bool useTexture; ///< Boolean value that determines if we render the texture.
uniform vec3 camera_pos; ///< Camera position.



void main()
{
  if(useTexture)
    FragColor =  texture(texture1, tex_coord);
  else
    FragColor = vec4(0.5,0.2,0.7,1);
}
