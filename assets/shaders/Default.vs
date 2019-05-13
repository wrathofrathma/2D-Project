#version 420 core


layout(location = 0) in vec3 vpos;
layout(location = 1) in vec2 text_uv;

uniform mat4 model; // Model matrix
uniform mat4 view; // View matrix
uniform mat4 proj; // Projection matrix

out vec4 frag_pos; ///< Our vertex position in model coordinates.
out vec2 tex_coord; ///< Texture coordinates.

void main(){
  frag_pos = model * vec4(vpos,1); // Converting to world position.
  tex_coord = text_uv;
//  gl_Position = proj * view * model * vec4(vpos,1,1);
//  gl_Position = proj* view * model * vec4(vpos,1);
gl_Position = proj * view* model * vec4(vpos,1);
}
