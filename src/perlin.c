#include <perlin.h>

int noise(int x, int y, int SEED) {
  return hash[((hash[(y + SEED) % 256]) + x) % 256];
}

float linear_interpolation(float x, float y, float s) {
  return x + s * (y - x);
}

float smooth_interpolation(float x, float y, float s) {
  return linear_interpolation(x, y, s * s * (3 - 2 * s));
}

float noise2d(float x, float y, int SEED) {
  float x_frac = x - ((int) x);
  float y_frac = y - ((int) y);
  int s = noise((int) x, (int) y, SEED);
  int t = noise(((int) x) + 1, (int) y, SEED);
  int u = noise((int) x, ((int) y) + 1, SEED);
  int v = noise(((int) x) + 1, ((int) y) + 1, SEED);
  float low = smooth_interpolation(s, t, x_frac);
  float high = smooth_interpolation(u, v, x_frac);
  return smooth_interpolation(low, high, y_frac);
}

float perlin(float x, float y, float freq, int depth, int SEED) {
  float xa = x * freq;
  float ya = y * freq;
  float amp = 1.0;
  float fin = 0.0;
  float div = 0.0;

  for (int i = 0; i < depth; i++) {
    div += 256 * amp;
    fin += noise2d(xa, ya, SEED) * amp;
    amp /= 2;
    xa *= 2;
    ya *= 2;
  }
  return fin/div;
}
