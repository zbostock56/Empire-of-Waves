/* INCLUDES */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <lexer_str.h>

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
void number(TOKEN *, TOKEN *, char);
void slash_or_comment(TOKEN *, char);
void init_lexer();
void tokenize(char *, int);
void print_tokens();

/* ======================= INTERNALLY DEFINED FUNCTIONS ==================== */
void parse();
