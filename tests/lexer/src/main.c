#include <m.h>

int main() {
  init_lexer();
  char buffer[40] = "THE QUICK BROWN FOX 123;\n //COMMENT";
  buffer[35] = '\0';
  int input_size = 36;
  tokenize(buffer, input_size);
  print_tokens();
  return 0;
}
