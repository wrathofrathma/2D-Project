#version 420 core



in vec4 frag_pos; ///< Fragment position
in vec2 tex_coord; ///< Texture coordinates.

out vec4 FragColor; ///< Our resulting color.

uniform sampler2D texture1; ///< Texture to use if we're using one.
uniform bool use_texture; ///< Boolean value that determines if we render the texture.
uniform vec3 camera_pos; ///< Camera position.
uniform float eye_angle;
uniform vec3 head_pos;
uniform vec4 global_ambient;
uniform bool use_lighting;
uniform vec3 eye_pos;
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
    float ambient_radius = 8*32; /// 8 block sight radius
    float eye_radius = 8*64;
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
    //If our distance from the head is within a certain radius, we illuminate the surrounding area so the user can see
    //As well as apply a spot light type effect with cutoffs within a certain angle.
    float PI = 3.14159265;
    float cutoff = PI/24.0; //If we're within PI/6 of our eye, we're going to apply a spot light from the origin of the eye.

    vec4 eye_color = vec4(1,0,0,1);

    if(dist < ambient_radius){
     total+= radial_lighting*atten;
    }
    if(dist < eye_radius){
      double angle = atan(frag_pos.y - eye_pos.y, frag_pos.x - eye_pos.x);
      if(angle<0)
        angle = (2*PI + angle);
      if(abs(eye_angle - angle) < cos(cutoff))
        FragColor += vec4(0.6,0,0,1);
    }
    FragColor = FragColor * total;
  }
  else
    FragColor *= global_ambient;


}
