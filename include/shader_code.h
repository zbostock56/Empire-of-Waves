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
"out vec4 FragColor;\n"
"in vec2 tex_coords;\n"
"uniform sampler2D tex;\n"
"uniform float threshold;\n"
"uniform float chunk;\n"
"#define C_WIDTH " TO_STRING(C_WIDTH) "\n"
"#define PI (3.14159265)\n"
"#define A " TO_STRING(A) "\n"
"#define B " TO_STRING(B) "\n"
"#define C " TO_STRING(C) "\n"
"#define K " TO_STRING(K) "\n"
"float first(float);\n"
"float second(float);\n"
"void main() {\n"
"  ivec2 tile_coords = {\n"
"    int(C_WIDTH * tex_coords.x),\n"
"    int(C_WIDTH * (1.0 - tex_coords.y))\n"
"  };\n"
"  int tile_index = (tile_coords.y * C_WIDTH) + tile_coords.x;\n"
"  float x = float(tile_index) / float(C_WIDTH * C_WIDTH);\n"
"  float u = first(x);\n"
"  float v = second(x);\n"
"  if (threshold < u || threshold > v) {\n"
"    FragColor = vec4(1.0);\n"
"  } else {\n"
"    FragColor = chunk * texture(tex, tex_coords);\n"
"  }\n"
"}\n"
"\n"
"float first(float x) {\n"
"  float num_tiles = C_WIDTH * C_WIDTH;\n"
"  float exp = pow(x+3.0,K);\n"
"  float sin_1 = pow(abs(sin(2.0*B*(x+15.0))), exp);\n"
"  float sin_2 = pow(abs(sin(PI*B*(x+15.0))), exp);\n"
"  return (A/2.0)*(sin_1+sin_2);\n"
"}\n"
"\n"
"float second(float x) {\n"
"  float num_tiles = C_WIDTH * C_WIDTH;\n"
"  float exp = pow(x+3.0,K);\n"
"  float sin_1 = pow(abs(sin(2.0*B*(x+17.0+(1.5*PI)))), exp);\n"
"  float sin_2 = pow(abs(sin(PI*B*(x+17.0+(1.5*PI)))), exp);\n"
"  return ((-A/2.0)*(sin_1+sin_2))+C;\n"
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
// "#version 430 core\n"
// "#define OCEAN (0)\n"
// "#define SHORE (1)\n"
// "#define SAND (2)\n"
// "#define GRASS (3)\n"
// "#define ROCK (4)\n"
// "#define MERCH (5)\n"
// "#define HOME (6)\n"
// "#define I_WIDTH " TO_STRING(I_WIDTH) "\n"
// "#define TEX_WIDTH (2)\n"
// "out vec4 FragColor;\n"
// "in vec2 tex_coords;\n"
// "uniform sampler2D ocean;\n"
// "uniform sampler2D shore;\n"
// "uniform sampler2D sand;\n"
// "uniform sampler2D grass;\n"
// "uniform sampler2D stone;\n"
// "uniform int tiles[I_WIDTH * I_WIDTH];\n"
// "void main() {\n"
// "  ivec2 tile_coords = {\n"
// "    int(I_WIDTH * tex_coords.x),\n"
// "    int(I_WIDTH * (1.0 - tex_coords.y))\n"
// "  };\n"
// "  vec2 tile_tex_coords = {\n"
// "    ((I_WIDTH * tex_coords.x) - tile_coords.x) / TEX_WIDTH,\n"
// "    ((I_WIDTH * tex_coords.y) - tile_coords.y) / TEX_WIDTH\n"
// "  };\n"
// "  tile_tex_coords.x -= int(tile_tex_coords.x);\n"
// "  tile_tex_coords.y -= int(tile_tex_coords.y);\n"
// "  tile_tex_coords.x += (float(tile_coords.x % TEX_WIDTH) / TEX_WIDTH);\n"
// "  tile_tex_coords.y += (float(tile_coords.y % TEX_WIDTH) / TEX_WIDTH);\n"
// "  int tile_index = (tile_coords.y * I_WIDTH) + tile_coords.x;\n"
// "  int tile = tiles[tile_index];\n"
// "  if (tile == OCEAN) {\n"
// "    FragColor = texture(ocean, tile_tex_coords);\n"
// "  } else if (tile == SHORE) {\n"
// "    FragColor = texture(shore, tile_tex_coords);\n"
// "  } else if (tile == SAND) {\n"
// "    FragColor = texture(sand, tile_tex_coords);\n"
// "  } else if (tile == GRASS) {\n"
// "    FragColor = texture(grass, tile_tex_coords);\n"
// "  } else if (tile == ROCK) {\n"
// "    FragColor = texture(stone, tile_tex_coords);\n"
// "  } else if (tile == MERCH) {\n"
// "    FragColor = texture(grass, tile_tex_coords);\n"
// "  } else if (tile == HOME) {\n"
// "    FragColor = texture(grass, tile_tex_coords);\n"
// "  } else {\n"
// "    FragColor = texture(ocean, tile_tex_coords);\n"
// "  }\n"
// "}\n";
