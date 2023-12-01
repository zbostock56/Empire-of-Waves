#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <lexer_str.h>
#include <parser_str.h>
#include <globals.h>
#include <float.h>

/* DEFINES */
#define MAX_TOKS (10)
#define GIVE ("give")
#define TELEPORT ("tp")
#define SPAWN ("spawn")
#define SET ("set")
#define HUNGER ("hunger")
#define TIMER ("timer")
#define ENEMY ("enemy")
#define SPEED ("speed")
#define TELEPORT_NEAREST_ISLAND ("tni")
#define MERCENARY ("mercenaries")
#define HOME ("home")
#define COORDS ("coords")
#define TELEPORT_NEAREST_MERCHANT ("tnm")
#define ESTABLISH_NEAREST_TRADEROUTE ("ent")
#define EVENT ("event")
#define HEALTH ("health")
#define WEATHER_TOK ("weather")
#define CLEAR_TOK ("clear")
#define FOG_TOK ("fog")
#define INVASION ("invasion")

/* GLOBALS */
extern LEXER lexer;
CURRENT current;
TOKEN command[MAX_TOKS];
extern int coords_enabled;

/* ================ INTERNALLY DEFINED FUNCTIONS ===================*/
void parse();
TOKEN get_token();
enum KIND peek_token();
void console_dispatcher();
void init_parse_table();
void print_parse_table();
float create_float(int);

/* ================ EXTERNALLY DEFINED FUNCTIONS ===================*/
void print_tokens();
void command_not_found();
void teleport(ivec2);
void teleport_nearest_island();
void set_speed(float);
void give_mercenaries(int);
void spawn_enemy();
void teleport_home();
void close_coords();
void teleport_nearest_merchant();
void set_hunger_level(float);
void set_hunger_timer(float);
void set_prompt(const char *);
void set_event_timer(float);
void set_health(float);
void set_weather(int);
void spawn_invasion();
//void establish_nearest_traderoute();
