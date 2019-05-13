#version 440 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout(location = 0) in vec4 vpos;
layout(location = 1) in vec3 vnormal;
layout(location = 2) in vec3 vcolor;
layout(location = 3) in vec2 text_uv;

in vec4 vert_pos;
in vec3 vert_norm;
in vec3 vert_color;
in vec2 vert_uv;

out vec4 frag_pos;
out vec3 normal;
out vec3 icolor;
out vec2 tex_coord;


void main()
{
  frag_pos = vert_pos;
  normal = vert_norm;
  icolor = vert_color;
  tex_coord = vert_uv;

  for(int i = 0; i < gl_in.length(); i++) {
      // copy attributes
      gl_Position = gl_in[i].gl_Position;

      // geo_stage_color = vert_stage_color;
      EmitVertex();
  }
  EndPrimitive();
}
