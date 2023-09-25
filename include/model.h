#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <cglm/cglm.h>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <model_str.h>

typedef struct mesh_vertex {
  vec3 coords;
  vec3 normal;
  vec2 tex_coords;
} MESH_VERT;

typedef struct mesh_data {
  MESH_VERT *vertices;
  int *indices;
  size_t num_verts;
  size_t num_indices;
} MESH_DATA;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

MESH_DATA read_mesh(char *);
void model_init(MESH_DATA *, MODEL *);

