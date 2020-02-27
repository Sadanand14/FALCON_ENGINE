#version 450 core
 in vec2 UV;
  out vec4 color;
  uniform sampler2D tex[2];

  void main()
  {
    color = texture(tex[0], UV);
    float depth = texture(tex[1], UV).r;

    // Ambient Occlusion
    vec2 r = 4.0 / textureSize(tex[0], 0);
    float occlusion = 0.0;
    for (int i = -2; i < 3; i++)
    {
      for (int j = -2; j < 3; j++)
      {
        occlusion += 1.0 / (1.0 + pow(10.0 * min(depth - texture(tex[1], UV + vec2(i, j) * r).r, 0.0), 2.0)) / 24.0;
      }
    }
    color.rgb *= occlusion;

    // Gamma Correction
    color.rgb = pow(1.0 - exp(-1.3 * color.rgb), vec3(1.3));
  }