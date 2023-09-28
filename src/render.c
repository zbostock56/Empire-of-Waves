#include <render.h>

void init_scene() {
  vec2 player_coords = { 0.0f, 0.0f };
  world_to_chunk(player_coords, e_player.ship_chunk, e_player.ship_coords);
  world_to_chunk(player_coords, e_player.chunk, e_player.coords);
  glm_vec2_zero(e_player.direction);
  glm_vec2_zero(e_player.ship_direction);
  e_player.direction[1] = 1.0;
  e_player.ship_direction[1] = 1.0;
  e_player.embarked = 1;

  // TEST MODELS
  vec2 te_coords = { 1.0f, 0.0f };
  world_to_chunk(te_coords, test_enemy.chunk, test_enemy.coords);
  glm_vec2_zero(test_enemy.direction);
  test_enemy.direction[0] = 1.0;

  vec2 ts_coords = { -1.5f, -1.5f };
  world_to_chunk(ts_coords, test_ts.chunk, test_ts.coords);
  glm_vec2_zero(test_ts.direction);
  test_ts.direction[0] = 1.0;
  // END TEST

  // Do scene initialization here
  // - load shaders
  // - load models
  // - etc..
  entity_framebuffer = framebuffer_init();
  model_shader = shader_init(vertex_shader_model, fragment_shader);
  pvm_shader = shader_init(vertex_shader_pvm, fragment_shader);
  player = load_model("assets/player.bin", "assets/3A.png");
  enemy = load_model("assets/enemy.bin", "assets/2A.png");
  merchant = load_model("assets/merchant.bin", "assets/2A.png");
  player_ship = load_model("assets/player_ship.bin", "assets/1A.png");
  enemy_ship = load_model("assets/enemy_ship.bin", "assets/1B.png");
  trade_ship = load_model("assets/trade_ship.bin", "assets/2A.png");
  quad = load_model("assets/quad.bin", NULL);

  // Setup perspective matrices
  glm_ortho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0, ortho_proj);
  glm_perspective(glm_rad(45.0f), RES_X / RES_Y, 0.1, 100.0, persp_proj);

  glEnable(GL_DEPTH_TEST);
}

void render_scene(GLFWwindow *window) {
  float current_frame = glfwGetTime();
  delta_time = current_frame - last_frame;
  last_frame = current_frame;

  glClearColor(1.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  render_player_ship();
  render_player();
  render_enemy_ship(&test_enemy);
  render_trade_ship(&test_ts);

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void render_player_ship() {
  if (mode == EXPLORATION) {
    mat4 fbo_model_mat = GLM_MAT4_IDENTITY_INIT;
    mat4 player_rot = GLM_MAT4_IDENTITY_INIT;
    vec3 player_dir = { e_player.ship_direction[0],
                        e_player.ship_direction[1], 0.0f };
    calc_rot_mat(player_dir, player_rot);
    glm_translate_y(fbo_model_mat, -0.5);
    glm_rotate_x(fbo_model_mat, glm_rad(-25.0), fbo_model_mat);
    glm_mat4_mul(fbo_model_mat, player_rot, fbo_model_mat);

    mat4 fbo_view_mat = GLM_MAT4_IDENTITY_INIT;
    glm_translate_z(fbo_view_mat, -3.0);

    mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
    mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
    if (!e_player.embarked) {
      vec3 world_coords = GLM_VEC3_ZERO_INIT;
      chunk_to_world(e_player.ship_chunk, e_player.ship_coords, world_coords);
      glm_translate(model_mat, world_coords);

      vec3 player_world_coords = GLM_VEC3_ZERO_INIT;
      chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
      glm_vec3_negate(player_world_coords);
      glm_translate(view_mat, player_world_coords);
    }
    glm_rotate_z(model_mat, glm_rad(90.0), model_mat);
    glm_scale_uni(model_mat, 0.25);


    render_fbo_entity(player_ship, fbo_model_mat, model_mat, fbo_view_mat,
                      view_mat, persp_proj, ortho_proj);
  }
}

void render_enemy_ship(E_ENEMY *es) {
  if (mode == EXPLORATION) {
    mat4 fbo_model_mat = GLM_MAT4_IDENTITY_INIT;
    mat4 enemy_rot = GLM_MAT4_IDENTITY_INIT;
    vec3 enemy_dir = { es->direction[0], es->direction[1], 0.0f };
    calc_rot_mat(enemy_dir, enemy_rot);
    glm_translate_y(fbo_model_mat, -0.5);
    glm_rotate_x(fbo_model_mat, glm_rad(-50.0), fbo_model_mat);
    glm_scale_uni(fbo_model_mat, 1.25);
    glm_mat4_mul(fbo_model_mat, enemy_rot, fbo_model_mat);

    mat4 fbo_view_mat = GLM_MAT4_IDENTITY_INIT;
    glm_translate_z(fbo_view_mat, -3.0);

    mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
    vec3 world_coords = GLM_VEC3_ZERO_INIT;
    chunk_to_world(es->chunk, es->coords, world_coords);
    glm_translate(model_mat, world_coords);
    glm_rotate_z(model_mat, glm_rad(90.0), model_mat);
    glm_scale_uni(model_mat, 0.25);

    mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
    vec3 player_world_coords = GLM_VEC2_ZERO_INIT;
    if (e_player.embarked) {
      chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                     player_world_coords);
    } else {
      chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
    }
    glm_vec3_negate(player_world_coords);
    glm_translate(view_mat, player_world_coords);

    render_fbo_entity(enemy_ship, fbo_model_mat, model_mat, fbo_view_mat,
                      view_mat, persp_proj, ortho_proj);
  }
}

void render_trade_ship(TRADE_SHIP *ts) {
  if (mode == EXPLORATION) {
    mat4 fbo_model_mat = GLM_MAT4_IDENTITY_INIT;
    mat4 ship_rot = GLM_MAT4_IDENTITY_INIT;
    vec3 ship_dir = { ts->direction[0], ts->direction[1],
                      0.0f };
    calc_rot_mat(ship_dir, ship_rot);
    glm_translate_y(fbo_model_mat, -0.5);
    glm_rotate_x(fbo_model_mat, glm_rad(-50.0), fbo_model_mat);
    glm_scale_uni(fbo_model_mat, 1.00);
    glm_mat4_mul(fbo_model_mat, ship_rot, fbo_model_mat);

    mat4 fbo_view_mat = GLM_MAT4_IDENTITY_INIT;
    glm_translate_z(fbo_view_mat, -3.0);

    mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
    vec3 world_coords = GLM_VEC3_ZERO_INIT;
    chunk_to_world(ts->chunk, ts->coords, world_coords);
    glm_translate(model_mat, world_coords);
    glm_rotate_z(model_mat, glm_rad(90.0), model_mat);
    glm_scale_uni(model_mat, 0.25);

    mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
    vec3 player_world_coords = GLM_VEC2_ZERO_INIT;
    if (e_player.embarked) {
      chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                     player_world_coords);
    } else {
      chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
    }
    glm_vec3_negate(player_world_coords);
    glm_translate(view_mat, player_world_coords);

    render_fbo_entity(trade_ship, fbo_model_mat, model_mat, fbo_view_mat,
                      view_mat, persp_proj, ortho_proj);
  }
}

void render_player() {
  if ((mode == EXPLORATION && !e_player.embarked) || mode == COMBAT) {
    mat4 fbo_model_mat = GLM_MAT4_IDENTITY_INIT;
    mat4 player_rot = GLM_MAT4_IDENTITY_INIT;
    vec3 player_dir = GLM_VEC2_ZERO_INIT;
    if (mode == EXPLORATION) {
      player_dir[0] = e_player.direction[0];
      player_dir[1] = e_player.direction[1];
    } else {
      player_dir[0] = c_player.direction[0];
      player_dir[1] = c_player.direction[1];
    }
    calc_rot_mat(player_dir, player_rot);
    glm_translate_y(fbo_model_mat, -0.5);
    glm_rotate_x(fbo_model_mat, glm_rad(-25.0), fbo_model_mat);
    glm_mat4_mul(fbo_model_mat, player_rot, fbo_model_mat);

    mat4 fbo_view_mat = GLM_MAT4_IDENTITY_INIT;
    glm_translate_z(fbo_view_mat, -3.0);

    mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
    glm_rotate_z(model_mat, glm_rad(90.0), model_mat);
    glm_scale_uni(model_mat, 0.25);

    mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

    render_fbo_entity(player, fbo_model_mat, model_mat, fbo_view_mat,
                      view_mat, persp_proj, ortho_proj);
  }
}

/*
void render_unit(C_UNIT *unit) {

}

void render_merchant(MERCHANT *merchant) {

}
*/

void render_fbo_entity(
    MODEL *entity,
    mat4 fbo_model, mat4 model,
    mat4 fbo_view, mat4 view,
    mat4 fbo_proj, mat4 proj
) {
  glBindFramebuffer(GL_FRAMEBUFFER, entity_framebuffer.FBO);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glUseProgram(pvm_shader);
  set_mat4("model", fbo_model, pvm_shader);
  set_mat4("view", fbo_view, pvm_shader);
  set_mat4("proj", fbo_proj, pvm_shader);
  draw_model(entity, pvm_shader);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  set_mat4("model", model, pvm_shader);
  set_mat4("view", view, pvm_shader);
  set_mat4("proj", proj, pvm_shader);
  quad->texture = entity_framebuffer.color_texture;
  draw_model(quad, pvm_shader);
}

/*
  Helper function for compiling a shader program
*/
unsigned int shader_init(const char *vs, const char *fs) {
  int vs_len = strlen(vs);
  unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shader, 1, &vs, &vs_len);
  glCompileShader(vert_shader);

  int status = 0;
  char info_log[1024];
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int info_log_len = 0;
    glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_log_len);
    glGetShaderInfoLog(vert_shader, 1024, &info_log_len, info_log);
    fprintf(stderr, "Vertex shader error:\n%s\n", info_log);

    return INVALID_SHADER;
  }

  int fs_len = strlen(fs);
  unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, &fs, &fs_len);
  glCompileShader(frag_shader);

  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int info_log_len = 0;
    glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &info_log_len);
    glGetShaderInfoLog(frag_shader, 1024, &info_log_len, info_log);
    fprintf(stderr, "Fragment shader error:\n%s\n", info_log);

    return INVALID_SHADER;
  }

  unsigned int shader_prog = glCreateProgram();
  glAttachShader(shader_prog, vert_shader);
  glAttachShader(shader_prog, frag_shader);
  glLinkProgram(shader_prog);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  return shader_prog;
}

/*
  Helper function to set up framebuffer and respective color, depth and stencil
  textures
*/
FRAMEBUFFER framebuffer_init() {
  FRAMEBUFFER fb;

  glGenFramebuffers(1, &fb.FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, fb.FBO);

  glGenTextures(1, &fb.color_texture);
  glBindTexture(GL_TEXTURE_2D, fb.color_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RES_X, RES_Y, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenRenderbuffers(1, &fb.depth_stencil_rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, fb.depth_stencil_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RES_X, RES_Y);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         fb.color_texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, fb.depth_stencil_rbo);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return fb;
}

void calc_rot_mat(vec3 dir, mat4 dest) {
  vec3 z_cross_dir = GLM_VEC3_ZERO_INIT;
  glm_vec3_cross(Z, dir, z_cross_dir);
  mat3 rot = GLM_MAT3_IDENTITY_INIT;
  glm_vec3_copy(z_cross_dir, rot[0]);
  glm_vec3_copy(dir, rot[1]);
  glm_vec3_copy(Z, rot[2]);
  glm_mat4_identity(dest);
  glm_mat4_ins3(rot, dest);
}

/*
  The following functions are helpers to consicely set up uniform variables in
  shaders
*/
void set_mat4(char *name, mat4 matrix, unsigned int shader) {
  glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE,
                     (float *) matrix);
}

void set_mat3(char *name, mat3 matrix, unsigned int shader) {
  glUniformMatrix3fv(glGetUniformLocation(shader, name), 1, GL_FALSE,
                     (float *) matrix);
}

void set_vec4(char *name, vec4 matrix, unsigned int shader) {
  glUniform4fv(glGetUniformLocation(shader, name), 1, (float *) matrix);
}

void set_vec3(char *name, vec3 matrix, unsigned int shader) {
  glUniform3fv(glGetUniformLocation(shader, name), 1, (float *) matrix);
}
