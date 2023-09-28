typedef struct model {
  unsigned int num_indices;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int texture;
} MODEL;

typedef struct character {
  MODEL *model;
  float width;
  float height;
} CHARACTER;

#define INVALID_TEXTURE (0xBAADF00D)
