/* INCLUDES */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* DEFINES */
#define MAX_LINES (512)
#define MAX_TOK_LEN (128)
#define MAX_TOKENS (128)
#define MAX_INPUT_SIZE (8096)

/* STRUCTS */
 enum KIND {
 NUMBER,
 IDENTIFIER,
 LEFTPAREN,
 RIGHTPAREN,
 LEFTSQUARE,
 RIGHTSQUARE,
 LEFTCURLY,
 RIGHTCURLY,
 LESSTHAN,
 GREATERTHAN,
 EQUAL,
 PLUS,
 MINUS,
 ASTERISK,
 SLASH,
 HASH,
 DOT,
 COMMA,
 COLON,
 SEMICOLON,
 SINGLEQUOTE,
 DOUBLEQUOTE,
 COMMENT,
 PIPE,
 END,
 UNEXPECTED,
};

typedef struct input {
  char in[MAX_INPUT_SIZE];
  int input_size;
} INPUT;

typedef struct token {
  enum KIND kind;
  char tok[MAX_TOK_LEN];
  int num_chars;
} TOKEN;

typedef struct line {
  TOKEN tokens[MAX_TOKENS];
  int num_tokens;
} LINE;

typedef struct lexer {
  LINE lines[MAX_LINES];
  int num_lines;
} LEXER;

/* GLOBALS */
LEXER lexer;
INPUT input;

/* MACROS */
#define space_check(lines, tokens, chars) (lines < MAX_LINES  \
  && tokens < MAX_TOKENS                                      \
  && chars < MAX_TOK_LEN)                                     \

/* ======================= INTERNALLY DEFINED FUNCTIONS ==================== */
int is_digit(char);
int is_space(char);
int is_identifier_char(char);
void next(LEXER *);
char get();
char peek();
void identifier(TOKEN *, char);
void number(TOKEN *, char);
void slash_or_comment(TOKEN *, char);
void init_lexer();
void tokenize(char *);
