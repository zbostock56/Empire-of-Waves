#include <init.h>

/*

TESTING COPY, CHANGED TO SUIT TESTING

*/

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
  /* Initialize buffers */
  for (int i = 0; i < 9; i++) {
    player_chunks[i].enemies = malloc(sizeof(E_ENEMY) * 10);
  }

  window = glfwCreateWindow(RES_X, RES_Y, PROJECT_TITLE, NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return NULL;
  }

  glfwSetFramebufferSizeCallback(window, fb_size_callback);
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
