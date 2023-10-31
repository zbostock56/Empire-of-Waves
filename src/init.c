#include <init.h>

GLFWwindow *init_gl() {
  GLFWwindow *window;

  if (!glfwInit()) {
    return NULL;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  /* Start global seed */
  srand(glfwGetTime() * 10101);

  window = glfwCreateWindow(RES_X, RES_Y, PROJECT_TITLE, NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return NULL;
  }

  glfwSetFramebufferSizeCallback(window, fb_size_callback);
  glfwSetMouseButtonCallback(window, mouse_click);
  glfwSetCursorPosCallback(window, mouse_pos);
  glfwSetScrollCallback(window, mouse_scroll);

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    return NULL;
  }

  glViewport(0, 0, RES_X, RES_Y);

  return window;
}

int init_game(char *save_name) {
  copy_valid_path(save_name, game_save_name, strlen(save_name));
  game_save_name[strlen(save_name)] = '\0';

  init_player();
  init_menus();
  init_dialog();
  init_trade();
  init_status_bar();
  init_ui_lists();
  init_container_ui();
  init_timers();
  open_status_bar();

  int status = 0;
  status = init_save_menu();
  if (status) {
    return -1;
  }

  status = init_chunks();
  if (status) {
    return -1;
  }

  status = init_containers();
  if (status) {
    return -1;
  }

  status = init_trade_ship_buffers();
  if (status) {
    return -1;
  }
  return 0;
}

void free_game() {
  free_dialog();
  free_trade();
  free_status_bar();
  clear_chunk_buffer();
  free_containers();
  free_trade_ship_buffers();
}
