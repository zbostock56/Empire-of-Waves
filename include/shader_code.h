/*
                                SHADER_CODE.h
Initializes character strings containing shader code to be used by program.
Should only be included by render.h.
*/

#define str(s) #s
#define TO_STRING(s) str(s)


// Normal vertex shader that takes projection, view and model matrix
const char *vertex_shader = " \
  #version 430 core\n \
  layout (location = 0) in vec3 in_pos;\n \
  layout (location = 1) in vec3 in_norm;\n \
  layout (location = 2) in vec2 in_tex;\n \
  out vec2 tex_coords;\n \
  uniform mat4 model;\n \
  uniform mat4 view;\n \
  uniform mat4 proj;\n \
  void main() {\n \
    gl_Position = proj * view * model * vec4(in_pos, 1.0);\n \
    tex_coords = in_tex;\n \
  }\n \
";

const char *fragment_shader_texture = " \
  #version 430 core\n \
  out vec4 FragColor;\n \
  in vec2 tex_coords;\n \
  uniform sampler2D tex;\n \
  void main() {\n \
    FragColor = texture(tex, tex_coords);\n \
  }\n \
";

const char *fragment_shader_pixelated = " \
  #version 430 core\n \
  out vec4 FragColor;\n \
  in vec2 tex_coords;\n \
  uniform sampler2D tex;\n \
  void main() {\n \
    float num_pixels = 1024.0;\n \
    float pix_size = 10 * (1.0 / num_pixels);\n \
    vec2 coord = vec2(pix_size * floor(tex_coords.x / pix_size),\n \
                      pix_size * floor(tex_coords.y / pix_size));\n \
    vec4 color = texture(tex, coord);\n \
    if (color.x == 0.0 && color.y == 0.0 && color.z == 0.0) {\n \
      discard;\n \
    }\n \
    FragColor = texture(tex, coord);\n \
  }\n \
";

const char *fragment_shader_text = " \
  #version 430 core\n \
  out vec4 FragColor;\n \
  in vec2 tex_coords;\n \
  uniform sampler2D tex;\n \
  void main() {\n \
    float col = texture(tex, tex_coords).r;\n \
    if (col == 0.0) {\n \
      discard;\n \
    } else {\n \
      FragColor = vec4(vec3(col), 1.0);\n \
    }\n \
  }\n \
";

const char *fragment_shader_color = " \
  #version 430 core\n \
  out vec4 FragColor;\n \
  in vec2 tex_coords;\n \
  uniform vec3 color;\n \
  void main() {\n \
    FragColor = vec4(color, 1.0);\n \
  }\n \
";

const char *fragment_shader_chunk =
"#version 430 core\n"
"#define C_WIDTH " TO_STRING(C_WIDTH) "\n"
"#define T_WIDTH (8)\n"
"#define PI " TO_STRING(PI) "\n"
"#define E (2.71828)\n"
"#define RANGE (5.0)\n"
"#define A " TO_STRING(A) "\n"
"#define B " TO_STRING(B) "\n"
"#define C " TO_STRING(C) "\n"
"#define K " TO_STRING(K) "\n"
"out vec4 FragColor;\n"
"in vec2 tex_coords;\n"
"uniform float radius;\n"
"uniform float threshold;\n"
"uniform float offset_1;\n"
"uniform float offset_2;\n"
"//uniform float chunk;\n"
"float first(float);\n"
"float second(float);\n"
"float third(float);\n"
"float fourth(float);\n"
"void main() {\n"
"  ivec2 pixel_coords = {\n"
"    int(C_WIDTH * T_WIDTH * tex_coords.x),\n"
"    int(C_WIDTH * T_WIDTH * (1.0 - tex_coords.y))\n"
"  };\n"
"  int pixel_index = (pixel_coords.y * C_WIDTH * T_WIDTH) + pixel_coords.x;\n"
"  float x = RANGE * (float(pixel_index) / (C_WIDTH * T_WIDTH * C_WIDTH * T_WIDTH));\n"
"  float u = first(x+offset_1);\n"
"  float v = second(x+offset_2);\n"
"  float w = third(x+offset_1);\n"
"  float m = fourth(x+offset_2);\n"
"  if (u > threshold || w > threshold || v < threshold || m < threshold) {\n"
"    FragColor = vec4(1.0);\n"
"  } else {\n"
"    FragColor = /* chunk **/ vec4(0.0117, 0.6156, 0.9882, 1.0);\n"
"  }\n"
"}\n"
"\n"
"float first(float x) {\n"
"  float sin_1 = sin((2/B)*x);\n"
"  float sin_2 = sin((PI/B)*x);\n"
"  float sin_3 = sin((sqrt(2)/B)*x);\n"
"  float sin_4 = sin((sqrt(3)/B)*x);\n"
"  float exp = (sin_1+(sin_2*sin_3)+sin_4-4.0)*(sin_1+(sin_2*sin_3)+sin_4-4.0);\n"
"  return A*pow(E,-exp);\n"
"}\n"
"float second(float x) {\n"
"  float sin_1 = sin((2/B)*(x+C));\n"
"  float sin_2 = sin((PI/B)*(x+C));\n"
"  float sin_3 = sin((sqrt(2)/B)*(x+C));\n"
"  float sin_4 = sin((sqrt(3)/B)*(x+C));\n"
"  float exp = (sin_1+(sin_2*sin_3)+sin_4-4.0)*(sin_1+(sin_2*sin_3)+sin_4-4.0);\n"
"  return (2.0*K)-(A*pow(E,-exp));\n"
"}\n"
"float third(float x) {\n"
"  float sin_1 = sin((2/B)*(x+6.0));\n"
"  float sin_2 = sin((PI/B)*(x+6.0));\n"
"  float sin_3 = sin((sqrt(2)/B)*(x+6.0));\n"
"  float sin_4 = sin((sqrt(3)/B)*(x+6.0));\n"
"  float exp = (sin_1+(sin_2*sin_3)+sin_4-4.0)*(sin_1+(sin_2*sin_3)+sin_4-4.0);\n"
"  return A*pow(E,-exp);\n"
"}\n"
"float fourth(float x) {\n"
"  float sin_1 = sin((2/B)*(x+C+6.0));\n"
"  float sin_2 = sin((PI/B)*(x+C+6.0));\n"
"  float sin_3 = sin((sqrt(2)/B)*(x+C+6.0));\n"
"  float sin_4 = sin((sqrt(3)/B)*(x+C+6.0));\n"
"  float exp = (sin_1+(sin_2*sin_3)+sin_4-4.0)*(sin_1+(sin_2*sin_3)+sin_4-4.0);\n"
"  return (2.0*K)-(A*pow(E,-exp));\n"
"}\n";

const char *fragment_shader_island =
"#version 430 core\n"
"#define OCEAN (0)\n"
"#define SHORE (1)\n"
"#define SAND (2)\n"
"#define GRASS (3)\n"
"#define ROCK (4)\n"
"#define MERCH (5)\n"
"#define HOME (6)\n"
"#define I_WIDTH " TO_STRING(I_WIDTH) "\n"
"out vec4 FragColor;\n"
"in vec2 tex_coords;\n"
"uniform int tiles[I_WIDTH * I_WIDTH];\n"
"void main() {\n"
"  ivec2 tile_coords = {\n"
"    int(I_WIDTH * tex_coords.x),\n"
"    int(I_WIDTH * (1.0 - tex_coords.y))\n"
"  };\n"
"  int tile_index = (tile_coords.y * I_WIDTH) + tile_coords.x;\n"
"  int tile = tiles[tile_index];\n"
"  vec3 color;\n"
"  if (tile == OCEAN) {\n"
"    discard;\n"
"  } else if (tile == SHORE) {\n"
"    color = vec3(0.0117, 0.9215, 0.9892);\n"
"  } else if (tile == SAND) {\n"
"    color = vec3(0.9882, 0.9529, 0.6342);\n"
"  } else if (tile == GRASS) {\n"
"    color = vec3(0.0156, 0.8196, 0.1490);\n"
"  } else if (tile == ROCK) {\n"
"    color = vec3(0.3882, 0.3411, 0.2627);\n"
"  } else if (tile == MERCH) {\n"
"    color = vec3(0.0156, 0.8196, 0.1490);\n"
"  } else if (tile == HOME) {\n"
"    color = vec3(1.0, 0.0, 1.0);\n"
"  } else {\n"
"    discard;\n"
"  }\n"
"  FragColor = vec4(color, 1.0);\n"
"}\n";

// VISUAL EFFECTS

const char *fragment_shader_ripple =
"#version 430 core\n"
"#define T_WIDTH (8)\n"
"#define RIPPLE_WIDTH " TO_STRING(RIPPLE_WIDTH) "\n"
"#define OCEAN (0)\n"
"#define SHORE (1)\n"
"out vec4 FragColor;\n"
"in vec2 tex_coords;\n"
"uniform vec2 ship_pos;\n"
"uniform mat4 ship_rot;\n"
"uniform int tiles[RIPPLE_WIDTH * RIPPLE_WIDTH];\n"
"uniform float time;\n"
"uniform int moving;\n"
"int calc_ship_circles(ivec2);\n"
"int calc_ship_wake(ivec2);\n"
"void main() {\n"
"  ivec2 tile_coords = {\n"
"    int(RIPPLE_WIDTH * tex_coords.x),\n"
"    int(RIPPLE_WIDTH * (1.0 - tex_coords.y))\n"
"  };\n"
"  ivec2 pixel_coords = {\n"
"    int(RIPPLE_WIDTH * T_WIDTH * tex_coords.x),\n"
"    int(RIPPLE_WIDTH * T_WIDTH * (1.0 - tex_coords.y))\n"
"  };\n"
"  int tile_index = (tile_coords.y * RIPPLE_WIDTH) + tile_coords.x;\n"
"  if (tiles[tile_index] == OCEAN || tiles[tile_index] == SHORE) {\n"
"    if (moving == 1 && calc_ship_wake(pixel_coords) == 1) {\n"
"      FragColor = vec4(1.0);\n"
"      return;\n"
"    } else if (moving == 0 && calc_ship_circles(pixel_coords) == 1) {\n"
"      FragColor = vec4(1.0);\n"
"      return;\n"
"    }\n"
"  }\n"
"  discard;\n"
"}\n"
"int calc_ship_circles(ivec2 pix_coords) {\n"
"  float dist = 0.0;\n"
"  vec2 ship_pixel_coords = vec2(\n"
"    T_WIDTH * RIPPLE_WIDTH * ship_pos.x,\n"
"    T_WIDTH * RIPPLE_WIDTH * ship_pos.y\n"
"  );\n"
"  vec4 elipse = transpose(ship_rot) *\n"
"                vec4(normalize(pix_coords - ship_pixel_coords), 0.0, 1.0);\n"
"  elipse.y *= 2.0;\n"
"  elipse.x *= 0.75;\n"
"  elipse *= 10.0;\n"
"  float comp_dist_max = length(elipse);\n"
"  float comp_dist_min = 0.7 * comp_dist_max;\n"
"  dist = distance(ship_pixel_coords, pix_coords);\n"
"  float ripple = sin(dist+time);\n"
"  if (dist >= comp_dist_min && dist <= comp_dist_max &&\n"
"      ((ripple <= 1.0 && ripple >= 0.95) ||\n"
"       (ripple >= -1.0 && ripple <= -0.95))) {\n"
"    return 1;\n"
"  }\n"
"  return 0;\n"
"}\n"
"int calc_ship_wake(ivec2 pix_coords) {\n"
"  float dist = 0.0;\n"
"  vec2 ship_pixel_coords = vec2(\n"
"    T_WIDTH * RIPPLE_WIDTH * ship_pos.x,\n"
"    T_WIDTH * RIPPLE_WIDTH * ship_pos.y\n"
"  );\n"
"  vec4 ship_dir = ship_rot * vec4(0.0, 1.0, 0.0, 0.0);\n"
"  vec2 ship_head = ship_pixel_coords + 15.0*vec2(ship_dir);\n"
"  vec4 wake_space = transpose(ship_rot) *\n"
"                    vec4(pix_coords - ship_head, 0.0, 1.0);\n"
"  dist = distance(ship_head-(80*vec2(ship_dir)), pix_coords);\n"
"  float wake_max = 0.5 * wake_space.x;\n"
"  wake_max = -wake_max * wake_max;\n"
"  float wake_min = 0.2 * wake_space.x;\n"
"  wake_min = -(wake_min * wake_min) - 40.0;\n"
"  float ripple = sin(10.0 * (dist+time));\n"
"  if (wake_space.y <= wake_max && wake_space.y >= wake_min &&\n"
"      ((ripple <= 1.0 && ripple >= 0.9) ||\n"
"       (ripple >= -1.0 && ripple <= -0.9))) {\n"
"    return 1;\n"
"  }\n"
"  return 0;\n"
"}\n"
;

