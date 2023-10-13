/*
                                SHADER_CODE.h
Initializes character strings containing shader code to be used by program.
Should only be included by render.h.
*/

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
    //float num_pixels = 1024.0;\n \
    float num_pixels = 1424.0;\n \
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

