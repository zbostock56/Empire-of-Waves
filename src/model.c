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
MODEL *load_model(char *mesh_path, char *tex_path) {
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

  if (tex_path) {
    model->texture = gen_texture(tex_path);
  } else {
    model->texture = INVALID_TEXTURE;
  }

  return model;
}

/*
  MODEL object cleanup
*/
void free_model(MODEL *model) {
  glDeleteBuffers(1, &model->VBO);
  glDeleteBuffers(1, &model->EBO);
  glDeleteVertexArrays(1, &model->VAO);
  if (model->texture != INVALID_TEXTURE) {
    glDeleteTextures(1, &model->texture);
  }
  free(model);
}

/*
  Render model to screen
*/
void draw_model(MODEL *model, unsigned int shader) {
  glUseProgram(shader);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, model->texture);
  glUniform1i(glGetUniformLocation(shader, "tex"), 0);

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

unsigned int gen_texture(char *path) {
  unsigned int tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width;
  int height;
  int num_channels;
  stbi_set_flip_vertically_on_load(1);
  unsigned char *tex_data = stbi_load(path, &width, &height, &num_channels, 0);
  if (tex_data) {
    int format = GL_RGBA;
    if (num_channels == 1) {
      format = GL_RED;
    } else if (num_channels == 2) {
      format = GL_RG;
    } else if (num_channels == 3) {
      format = GL_RGB;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, tex_data);
  } else {
    fprintf(stderr, "model_loader.c: Failed to load texture: %s\n", path);
    return INVALID_TEXTURE;
  }
  stbi_image_free(tex_data);

  return tex;
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
