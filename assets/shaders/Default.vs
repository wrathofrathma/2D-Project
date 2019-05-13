#version 420 core
/**
\file Default.vs

\brief Default vertex shader.

\param [in] vpos --- vec4 vertex position.

\param [in] vcolor --- vec3 vertex color.

\param [in] vnormal --- vec3 normal vector.

\param [in] text_uv --- vec2 texture coordinates.

\param [in] model --- mat4 Model matrix.

\param [in] view --- mat4 View matrix.

\param [in] proj --- mat4 Projection matrix.

\param [in] time --- float containing the time used to offset our texture.

\param [in] normal_matrix --- mat3 normal matrix.

\param [out] frag_pos --- vec4 containing the fragment position.

\param [out] normal --- normal vector that's been modified with the model matrix.

\param [out] tex_coord --- vec2 texture coordinates.
*/

layout(location = 0) in vec4 vpos;
layout(location = 1) in vec3 vnormal;
layout(location = 2) in vec3 vcolor;
layout(location = 3) in vec2 text_uv;

uniform mat4 model; // Model matrix
uniform mat4 view; // View matrix
uniform mat4 proj; // Projection matrix
uniform mat3 normal_matrix; ///< Normal matrix.

out vec4 frag_pos; ///< Our vertex position in model coordinates.
out vec3 normal; ///< Our vertex normal.
out vec3 icolor; ///< Our input surface color if one exists.
out vec2 tex_coord; ///< Texture coordinates.

void main(){
  normal = normal_matrix * vnormal;
  frag_pos = model * vpos; // Converting to world position.
  icolor = vcolor;
  tex_coord = text_uv;
  gl_Position = proj * view * model * vpos;
}
