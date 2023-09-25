#include <model.h>
/*
                                  MODEL.c
Implements the functionality for model loading, and model drawing. model.h
defines the struct maintaining OpenGL buffer information for the imported
model.
*/

/*
  Loads a 3D model's mesh data into memory to populate a new MODEL object
*/
MODEL *load_model(char *mesh_path) {
  MODEL *model = malloc(sizeof(MODEL));
  if (model == NULL) {
    return NULL;
  }

  MESH_DATA md = read_mesh(mesh_path);
  if (!md.vertices && !md.indices && !md.num_verts && !md.num_indices) {
    free(model);
    return NULL;
  }

  model_init(&md, model);

  return model;
}

/*
  MODEL object cleanup
*/
void free_model(MODEL *model) {
  glDeleteBuffers(1, &model->VBO);
  glDeleteBuffers(1, &model->EBO);
  glDeleteVertexArrays(1, &model->VAO);
  free(model);
}

/*
  Render model to screen
*/
void draw_model(MODEL *model) {
  glBindVertexArray(model->VAO);
  glDrawElements(GL_TRIANGLES, model->num_indices, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

MESH_DATA read_mesh(char *path) {
  MESH_DATA md = {
    NULL,
    NULL,
    0,
    0
  };

  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    return md;
  }

  fread(&md.num_verts, sizeof(size_t), 1, file);
  fread(&md.num_indices, sizeof(size_t), 1, file);

  if (md.num_verts) {
    md.vertices = malloc(sizeof(MESH_VERT) * md.num_verts);
    if (md.vertices == NULL) {
      fclose(file);
      md.num_verts = 0;
      md.num_indices = 0;
      fprintf(stderr,
              "model_loader.c: Failed to allocate vertices for model: %s\n",
              path);
      return md;
    }
    fread(md.vertices, sizeof(MESH_VERT), md.num_verts, file);
  }

  if (md.num_indices) {
    md.indices = malloc(sizeof(int) * 3 * md.num_indices);
    if (md.indices == NULL) {
      fclose(file);
      free(md.vertices);
      md.vertices = NULL;
      md.num_verts = 0;
      md.num_indices = 0;
      fprintf(stderr,
              "model_loader.c: Failed to allocate indices for model: %s\n",
              path);
      return md;
    }
    fread(md.indices, sizeof(int) * 3, md.num_indices, file);
  }
  fclose(file);

  return md;
}

void model_init(MESH_DATA *md, MODEL *model) {
  size_t num_verts = md->num_verts * 8;
  size_t num_indices = md->num_indices * 3;

  glGenVertexArrays(1, &model->VAO);
  glBindVertexArray(model->VAO);

  glGenBuffers(1, &model->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_verts, md->vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) (3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) (6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &model->EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, md->indices,
               GL_STATIC_DRAW);

  model->num_indices = num_indices;

  glBindVertexArray(0);
}
