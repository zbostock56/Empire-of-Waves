#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <lexer_str.h>
#include <parser_str.h>
#include <globals.h>

/* DEFINES */
#define MAX_TOKS (10)
#define GIVE ("give")
#define TELEPORT ("tp")
#define SPAWN ("spawn")
#define SET ("set")
#define ENEMY ("enemy")
#define SPEED ("speed")
#define TELEPORT_NEAREST_ISLAND ("tni")
#define MERCENARY ("mercenaries")
#define HOME ("home")

/* GLOBALS */
extern LEXER lexer;
CURRENT current;
TOKEN command[MAX_TOKS];

/* ================ INTERNALLY DEFINED FUNCTIONS ===================*/
void parse();
TOKEN get_token();
enum KIND peek_token();
void console_dispatcher();
void init_parse_table();
void print_parse_table();

/* ================ EXTERNALLY DEFINED FUNCTIONS ===================*/
void print_tokens();
void command_not_found();
void teleport(ivec2);
void teleport_nearest_island();
void set_speed(float);
void give_mercenaries(int);
void spawn_enemy();
void teleport_home();
