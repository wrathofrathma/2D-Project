#version 420 core



in vec4 frag_pos; ///< Fragment position
in vec2 tex_coord; ///< Texture coordinates.

out vec4 FragColor; ///< Our resulting color.

uniform sampler2D texture1; ///< Texture to use if we're using one.
uniform bool use_texture; ///< Boolean value that determines if we render the texture.
uniform vec3 camera_pos; ///< Camera position.
uniform vec2 eye_angle;
uniform vec3 head_pos;
uniform vec4 global_ambient;
uniform bool use_lighting;

void main()
{
  if(use_texture){
    FragColor =  texture(texture1, tex_coord);
    if(FragColor.a<0.2)
      discard;
  }
  else
    FragColor = vec4(0.5,0.2,0.7,1);

  if(use_lighting){
    //Our head position can be used to calculate the attenuation factor as if the head itself were an omnidirectional point light
    vec4 total = vec4(0);
    vec4 radial_lighting = vec4(1);
    float radius = 8*32; /// 8 block sight radius

    vec3 head_diff = head_pos - frag_pos.xyz;
    //float dist = dot(head_diff, head_diff);
    float dist = length(head_pos - frag_pos.xyz);

    float aconst = 1.0;
    float alinear = 0.007;
    float aquad = 0.0002;
    float atten = 1.0 / (aconst + alinear * dist + aquad * (dist*dist));

    radial_lighting *= atten;
    total+= radial_lighting;
    vec4 ambient = global_ambient * atten;
    if(dist < radius)
      total+= radial_lighting;
    FragColor = FragColor * total;
  }
  else
    FragColor *= global_ambient;
}
