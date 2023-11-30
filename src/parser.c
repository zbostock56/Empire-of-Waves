#include <parser.h>

/*
GRAMMAR:
command := simple_command end | end
simple_command := identifier | identifier argument
argument := (identifier)* | (number)*
*/
void parse() {
  init_parse_table();
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
        (peek_token() == NUMBER || peek_token() == IDENTIFIER ||
         peek_token() == DOT)) {
    command[cur_tok++] = get_token();
  }
  if (peek_token() != END) {
    command_not_found();
    return;
  } else if (lexer.lines[lexer.num_lines].num_tokens == 0 && peek_token() == END) {
    command[cur_tok++] = get_token();
  }
  /* END: argument */
  /* END: simple_command */
  /* END: command */
  console_dispatcher();
}

/* Responsible for calling appropriate console command with arguments */
/* passed through to the respective function  */
void console_dispatcher() {
  if (command[0].kind == IDENTIFIER) {
    if (strncmp(command[0].tok, SET, sizeof(SET)) == 0) {
      /* BEGIN: set  */
      if (strncmp(command[1].tok, SPEED, sizeof(SPEED)) == 0) {
        /* Generate the float out of the token string */
        if (command[2].num_chars > 3) {
          set_prompt("Speed too high!");
          return;
        }
        float speed = create_float(2);
        if (speed == 0.0) {
          set_speed(1.0);
          return;
        } else if (speed == FLT_MAX) {
          set_prompt("Invalid Number");
          return;
        }
        set_speed(speed);
      } else if (strncmp(command[1].tok, HUNGER, sizeof(HUNGER)) == 0) {
        if (command[2].kind == NUMBER) {
          float hunger = create_float(2); 
          if (hunger == 0.0) {
            set_prompt("Invalid hunger level!");
            return;
          } else if (hunger == FLT_MAX) {
            set_prompt("Invalid Number");
            return;
          }
          set_hunger_level(hunger);
          return;
        } else if (strncmp(command[2].tok, TIMER, sizeof(TIMER)) == 0) {
          float timer = create_float(3);
          if (timer == 0.0 || timer == FLT_MAX) {
            set_prompt("Invalid number");
          }
          set_hunger_timer(timer);
          return;
        } else {
          command_not_found();
          return;
        } 
      } else if (strncmp(command[1].tok, HEALTH, sizeof(HEALTH)) == 0) {
          float health = create_float(2);
          if (health == 0.0) {
            set_prompt("Invalid number");
            return;
          } else if (health > e_player.max_health) {
            set_prompt("Over max available health");
            return;
          }
          set_health(health);
          return;
      } else {
        command_not_found();
      }
    /* END: set */
    } else if (strncmp(command[0].tok, TELEPORT_NEAREST_ISLAND, sizeof(TELEPORT_NEAREST_ISLAND)) == 0) {
      /* BEGIN: tni  */
      teleport_nearest_island();
      /* END: tni */
    } else if (strncmp(command[0].tok, TELEPORT, sizeof(TELEPORT)) == 0) {
      /* BEGIN: tp */
        if (command[1].kind == NUMBER && command[2].kind == NUMBER) {
          ivec2 loc = { atoi(command[1].tok), atoi(command[2].tok) };
          teleport(loc);
        } else if (command[1].kind == IDENTIFIER &&
                   strncmp(command[1].tok, HOME, sizeof(HOME)) == 0) {
           teleport_home();
        } else {
           command_not_found();
           print_parse_table();
        }
      /* END: tp */
    } else if (strncmp(command[0].tok, GIVE, sizeof(GIVE)) == 0) {
      /* BEGIN: give */
      if (command[1].kind == IDENTIFIER && strncmp(command[1].tok, MERCENARY, sizeof(MERCENARY)) == 0) {
        int num_mercenaries = atoi(command[2].tok);
        give_mercenaries(num_mercenaries);
      } else {
        command_not_found();
      }
      /* END: give */
    } else if (strncmp(command[0].tok, SPAWN, sizeof(SPAWN)) == 0) {
      /* BEGIN: spawn */
      if (command[1].kind == IDENTIFIER && strncmp(command[1].tok, ENEMY, sizeof(ENEMY)) == 0) {
        if (command[2].kind == NUMBER) {
          for (int i = 0; i < atoi(command[2].tok); i++) {
            spawn_enemy();
          }
        } else {
          spawn_enemy();
        }
      } else {
        command_not_found();
      }
      /* END: spawn  */
    } else if (strncmp(command[0].tok, COORDS, sizeof(COORDS)) == 0) {
      if (!coords_enabled) {
        coords_enabled = 1;
      } else {
        coords_enabled = 0;
        close_coords();
      }
    } else if (strncmp(command[0].tok, TELEPORT_NEAREST_MERCHANT,
               sizeof(TELEPORT_NEAREST_MERCHANT)) == 0) {
      /* BEGIN: tnm */
      teleport_nearest_merchant();
      /* END: tmn */
    } else if (strncmp(command[0].tok, ESTABLISH_NEAREST_TRADEROUTE,
               sizeof(ESTABLISH_NEAREST_TRADEROUTE)) == 0) {
      //establish_nearest_traderoute();
      return;
    } else {
      command_not_found();
    }
  } else {
    command_not_found();
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

float create_float(int start) {
  char fl[6];
  int index = command[start].num_chars;
  /* snprintf will fail if not accounting for null byte  */
  /* therefore need to add one to size to copy to work   */
  /* properly                                            */
  if (command[start].kind != NUMBER) {
    return FLT_MAX;
  }
  snprintf(fl, index + 1, "%s", command[start].tok);
  start++;
  fl[index++] = '.';
  start++;
  if (command[start].kind != NUMBER) {
    fl[index] = '0';
  } else {
    /* Only take the first and second decimal places and discard the rest */
    if (command[start].num_chars == 1) {
      fl[index++] = command[start].tok[0];
    } else {
      fl[index++] = command[start].tok[0];
      fl[index++] = command[start].tok[1];
    }
    while (index < 6) {
      fl[index++] = '0';
    }
  }
  return atof(fl);
}

void print_parse_table() {
  TOKEN cur;
  printf("\n");
  for (int i = 0; i < MAX_TOKS; i++) {
    cur = command[i];
    printf("TOKEN: %d\t(", i);
      switch (cur.kind) {
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
    for (int j = 0; j < cur.num_chars; j++) {
      if (cur.tok[j] == '\0') {
        printf(" ");
        continue;
      }
      printf("%c", cur.tok[j]);
    }
    printf("|)\n");
  }
}

void init_parse_table() {
  TOKEN dummy = {
    UNEXPECTED,
    "",
    0,
  };
  for (int i = 0; i < MAX_TOKS; i++) {
    command[i] = dummy;
  }
}
