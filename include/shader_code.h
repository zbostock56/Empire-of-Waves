/*
                                SHADER_CODE.h
Initializes character strings containing shader code to be used by program.
Should only be included by render.h.
*/

// Normal vertex shader that only takes in a model matrix
const char *vertex_shader_model = " \
  #version 430 core\n \
  layout (location = 0) in vec3 in_pos;\n \
  layout (location = 1) in vec3 in_norm;\n \
  layout (location = 2) in vec2 in_tex;\n \
  out vec2 tex_coords;\n \
  uniform mat4 model;\n \
  void main() {\n \
    gl_Position = model * vec4(in_pos, 1.0);\n \
    tex_coords = in_tex;\n \
  }\n \
";

// Normal vertex shader that takes projection, view and model matrix
const char *vertex_shader_pvm = " \
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


const char *fragment_shader = " \
  #version 430 core\n \
  out vec4 FragColor;\n \
  in vec2 tex_coords;\n \
  uniform sampler2D tex;\n \
  void main() {\n \
    FragColor = texture(tex, tex_coords);\n \
  }\n \
";
