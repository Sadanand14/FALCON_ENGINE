#version 450 core
out vec3 pos;
  out vec3 fsun;
  uniform mat4 P;
  uniform mat4 V;
  uniform float time = 0.0;

  const vec2 data[4] = vec2[](
    vec2(-1.0,  1.0), vec2(-1.0, -1.0),
    vec2( 1.0,  1.0), vec2( 1.0, -1.0));

  void main()
  {
    gl_Position = vec4(data[gl_VertexID], 0.0, 1.0);
    pos = transpose(mat3(V)) * (inverse(P) * gl_Position).xyz;
    fsun = vec3(0.0, sin(time * 0.01), cos(time * 0.01));
  }

