#include <lexer.h>

/*
                      LEXER.c
  Responsible for tokenizing input in the console
*/

void init_lexer() {
  input.input_size = 0;
  lexer.num_lines = 0;
  for (int i = 0; i < MAX_LINES; i++) {
    lexer.lines[i].num_tokens = 0;
    for (int j = 0; j < MAX_TOKENS; j++) {
      lexer.lines[i].tokens[j].num_chars = 0;
    }
  }
}

int is_digit(char c) {
  switch (c) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '0':
      return 1;
    default:
      return 0;
  }
}

int is_space(char c) {
  switch (c) {
    case ' ':
    case '\r':
    case '\t':
    case '\n':
      return 1;
    default:
      return 0;
  }
}

int is_identifier_char(char c) {
  switch(c) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '_':
      return 1;
    default:
      return 0;
  }
}

/* Main function to parse entire token */
void next(LEXER *lexer) {
  while (is_space(peek())) {
    get();
  }
  char c = get();
  int lines = lexer->num_lines;
  int toks = lexer->lines[lines].num_tokens;
  int characters = lexer->lines[lines].tokens[toks].num_chars;
  if (!space_check(lines, toks, characters)) {
    fprintf(stderr, "SPACE ERROR\n");
    exit(1);
  }
  switch (c) {
    case '\0':
      lexer->lines[lines].tokens[toks].kind = END;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      lexer->lines[lines].tokens[toks].num_chars++;
      lexer->lines[lines].num_tokens++;
      /* Line has been ended, increase number of lines */
      lexer->num_lines++;
      break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
      identifier(&(lexer->lines[lines].tokens[toks]), c);
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      number(&(lexer->lines[lines].tokens[toks]), c);
      break;
    case '(':
      lexer->lines[lines].tokens[toks].kind = LEFTPAREN;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case ')':
      lexer->lines[lines].tokens[toks].kind = RIGHTPAREN;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '[':
      lexer->lines[lines].tokens[toks].kind = LEFTSQUARE;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case ']':
      lexer->lines[lines].tokens[toks].kind = RIGHTSQUARE;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '{':
      lexer->lines[lines].tokens[toks].kind = LEFTCURLY;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '}':
      lexer->lines[lines].tokens[toks].kind = RIGHTCURLY;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '<':
      lexer->lines[lines].tokens[toks].kind = LESSTHAN;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '>':
      lexer->lines[lines].tokens[toks].kind = GREATERTHAN;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '=':
      lexer->lines[lines].tokens[toks].kind = EQUAL;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '+':
      lexer->lines[lines].tokens[toks].kind = PLUS;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '-':
      lexer->lines[lines].tokens[toks].kind = MINUS;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '*':
      lexer->lines[lines].tokens[toks].kind = ASTERISK;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '/':
      slash_or_comment(&(lexer->lines[lines].tokens[toks]), c);
      break;
    case '#':
      lexer->lines[lines].tokens[toks].kind = HASH;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '.':
      lexer->lines[lines].tokens[toks].kind = DOT;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case ',':
      lexer->lines[lines].tokens[toks].kind = COMMA;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case ':':
      lexer->lines[lines].tokens[toks].kind = COLON;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case ';':
      lexer->lines[lines].tokens[toks].kind = SEMICOLON;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '\'':
      lexer->lines[lines].tokens[toks].kind = SINGLEQUOTE;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '"':
      lexer->lines[lines].tokens[toks].kind = DOUBLEQUOTE;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    case '|':
      lexer->lines[lines].tokens[toks].kind = PIPE;
      lexer->lines[lines].tokens[toks].tok[characters] = c;
      break;
    default:
      fprintf(stderr, "UNEXPECTED TOKEN %c\n", peek());
      exit(1);
  }
  lexer->lines[lines].tokens[toks].num_chars++;
  lexer->lines[lines].num_tokens++;
}

/* Outputs the whole of a string as */
/* a token */
void identifier(TOKEN *token, char c) {
  token->kind = IDENTIFIER;
  token->tok[token->num_chars] = c;
  token->num_chars++;
  int lines = lexer.num_lines;
  int toks = lexer.lines[lines].num_tokens;
  int characters = token->num_chars;
  while (is_identifier_char(peek())) {
    if (!space_check(lines, toks, characters)) {
      fprintf(stderr, "SPACE ERROR\n");
      exit(1);
    }
    token->tok[token->num_chars] = get();
    token->num_chars++;
    lines = lexer.num_lines;
    toks = lexer.lines[lines].num_tokens;
    characters = token->num_chars;
  }
}

/* Outputs the whole of a number as */
/* a token */
void number(TOKEN *token, char c) {
  token->kind = NUMBER;
  token->tok[token->num_chars] = c;
  token->num_chars++;
  int lines = lexer.num_lines;
  int toks = lexer.lines[lines].num_tokens;
  int characters = token->num_chars;
  while (is_digit(peek())) {
    if (!space_check(lines, toks, characters)) {
      fprintf(stderr, "SPACE ERROR\n");
      exit(1);
    }
    token->tok[token->num_chars] = get();
    token->num_chars++;
    lines = lexer.num_lines;
    toks = lexer.lines[lines].num_tokens;
    characters = token->num_chars;
  }
}

/* Determines if the current token is  */
/* a slash or comment, then outputs as */
/* a token */
void slash_or_comment(TOKEN *token, char c) {
  token->tok[token->num_chars] = c;
  token->num_chars++;
  int lines = lexer.num_lines;
  int toks = lexer.lines[lines].num_tokens;
  int characters = token->num_chars;
  /* Could be [slash][slash] or [slash][asterisk]  */
  if (peek() == '/' || peek() == '*') {
    token->kind = COMMENT;
    while (peek() != '\0') {
      if (!space_check(lines, toks, characters)) {
        fprintf(stderr, "SPACE ERROR\n");
        exit(1);
      }
      if (peek() != '\n') {
        token->tok[token->num_chars] = get();
        token->num_chars++;
        lines = lexer.num_lines;
        toks = lexer.lines[lines].num_tokens;
        characters = token->num_chars;
      } else {
        lexer.num_lines++;
        return;
      }
    }
    //fprintf(stderr, "UNEXPECTED TOKEN WHEN PARSING COMMENT: %c\n", peek());
    //exit(1);
  } else {
    token->kind = SLASH;
    return;
  }
}

/* Get the next character off the buffer */
/* and increment pointer */
char get() {
  if (input.input_size - 1 < 0) {
    fprintf(stderr, "NO CHARACTERS LEFT TO GET\n");
    exit(1);
  }
  input.input_size--;
  char c = *input.in++;
  return c;
}

/* Look at the character on the front of */
/* the buffer */
char peek() {
  if (input.input_size < 0) {
    fprintf(stderr, "NO CHARACTERS TO PEEK AT\n");
    exit(1);
  }
  char c = *input.in;
  return c;
}

void tokenize(char *buffer, int buffer_size) {
  if (buffer_size == 0) {
    return;
  }
  if (buffer_size > MAX_INPUT_SIZE) {
    fprintf(stderr, "COMMAND INPUT TOO LARGE\n");
    exit(1);
  }
  input.in = malloc(buffer_size);
  char *temp = input.in;
  for (int i = 0; i < buffer_size; i++) {
    input.in[i] = buffer[i];
  }
  input.input_size = buffer_size;
  while (input.input_size > 0) {
    next(&lexer);
  }
  free(temp);
  parse();
}

void print_tokens() {
  for (int i = 0; i < lexer.num_lines; i++) {
    LINE l = lexer.lines[i];
    for (int j = 0; j < l.num_tokens; j++) {
      TOKEN t = l.tokens[j];
      switch (t.kind) {
        case NUMBER:
          printf("NUMBER      |");
          break;
        case IDENTIFIER:
          printf("IDENTIFIER  |");
          break;
        case LEFTPAREN:
          printf("LEFTPAREN   |");
          break;
        case RIGHTPAREN:
          printf("RIGHTPAREN  |");
          break;
        case LEFTSQUARE:
          printf("LEFTSQUARE  |");
          break;
        case RIGHTSQUARE:
          printf("RIGHTSQUARE |");
          break;
        case LEFTCURLY:
          printf("LEFTCURLY   |");
          break;
        case RIGHTCURLY:
          printf("RIGHTCURLY  |");
          break;
        case LESSTHAN:
          printf("LESSTHAN    |");
          break;
        case GREATERTHAN:
          printf("GREATERTHAN |");
          break;
        case EQUAL:
          printf("EQUAL       |");
          break;
        case PLUS:
          printf("PLUS        |");
          break;
        case MINUS:
          printf("MINUS       |");
          break;
        case ASTERISK:
          printf("ASTERISK    |");
          break;
        case SLASH:
          printf("SLASH       |");
          break;
        case HASH:
          printf("HASH        |");
          break;
        case DOT:
          printf("DOT         |");
          break;
        case COMMA:
          printf("COMMA       |");
          break;
        case COLON:
          printf("COLON       |");
          break;
        case SEMICOLON:
          printf("SEMICOLON   |");
          break;
        case SINGLEQUOTE:
          printf("SINGLEQUOTE |");
          break;
        case DOUBLEQUOTE:
          printf("DOUBLEQUOTE |");
          break;
        case COMMENT:
          printf("COMMENT     |");
          break;
        case PIPE:
          printf("PIPE        |");
          break;
        case END:
          printf("END         |");
          break;
        case UNEXPECTED:
          printf("UNEXPECTED  |");
          break;
        }
      for (int k = 0; k < t.num_chars; k++) {
        printf("%c", t.tok[k]);
      }
      printf("|\n");
    }
  }
}
