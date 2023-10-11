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
