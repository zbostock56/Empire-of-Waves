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
  char *in;
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
