#include <parser.h>

/*
GRAMMAR:
command := simple_command end | end
simple_command := identifier | identifier argument
argument := (identifier)* | (number)*
*/
void parse() {
  current.line = 0;
  current.token = 0;
  int cur_tok = 0;
  /* BEGIN: command  */
  /* Empty case should be handled by lexer */
  if (peek_token() == END) {
     return;
  }
  /* BEGIN: simple_command */
  if (peek_token() != IDENTIFIER) {
    command_not_found();
    return;
  }
  command[cur_tok++] = get_token();
  /* BEGIN: argument */
  while (lexer.lines[lexer.num_lines].num_tokens != current.token &&
        (peek_token() == NUMBER || peek_token() == IDENTIFIER)) {
    command[cur_tok++] = get_token();
  }
  if (lexer.lines[lexer.num_lines].num_tokens != 0 && peek_token() != END) {
    command_not_found();
    return;
  }
  /* END: argument */
  /* END: simple_command */
  /* END: command */
  console_dispatcher();
}

/* Responsible for calling appropriate console command with arguments */
/* passed through to the respective function  */
void console_dispatcher() {
  if (strncmp(command[0].tok, SET, sizeof(SET)) == 0) {
    return;
    if (strncmp(command[1].tok, SPEED, sizeof(SPEED)) == 0) {
      /* atof() returns 0.0 when it can't make a conversion */
      /* set to default speed if conversion can't be made */
      if (atof(command[2].tok) == 0.0) {
        set_speed(1.0);
      }
      set_speed(atof(command[2].tok));
    }
  } else if (strncmp(command[0].tok, SPAWN, sizeof(SPAWN)) == 0) {
    return;
  } else if (strncmp(command[0].tok, TELEPORT_NEAREST_ISLAND, sizeof(TELEPORT_NEAREST_ISLAND)) == 0) {
    teleport_nearest_island();
  }

}

/* Gets the next token off the stack */
TOKEN get_token() {
  if (current.line == lexer.num_lines) {
    fprintf(stderr, "NO MORE TOKENS TO GET FROM LEXER\n");
    exit(1);
  }
  if (current.token + 1 == lexer.lines[current.line].num_tokens) {
    TOKEN ret = lexer.lines[current.line++].tokens[current.token];
    current.token = 0;
    return ret;
  }
  return lexer.lines[current.line].tokens[current.token++];
}

/* Peeks the kind of token off the stack */
enum KIND peek_token() {
  if (current.line == lexer.num_lines) {
    fprintf(stderr, "NO MORE TOKENS LEFT TO PEEK FROM LEXER\n");
    exit(1);
  }
  return lexer.lines[current.line].tokens[current.token].kind;
}
