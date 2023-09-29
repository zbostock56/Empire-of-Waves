#include <console.h>


/*
Command List:
-> give
-> teleport
-> spawn_enemy
-> spawn_trade_ship

Assumptions:
  -> cmd is delimited by null terminator ('\0')
*/
void handle_command(char *cmd) {
  char *buffer[MAX_TOKENS];
  int cmd_len = strlen(cmd);
  char *cur_tok = cmd;
  int num_tokens = 0;
  for (int i = 0; i < cmd_len && num_tokens < MAX_TOKENS; i++) {
    if (cmd[i] == ' ') {
      cmd[i] = '\0';
      buffer[num_tokens] = cur_tok;
      cur_tok = cmd + i + 1;
      num_tokens++;
    } else if (i == cmd_len - 1) {
      buffer[num_tokens] = cur_tok;
      num_tokens++;
    }
  }
  if (num_tokens >= 1) {
    if (strncmp(buffer[0], GIVE, strlen(GIVE) + 1) == 0) {
      printf("GIVE\n");
    } else if (strncmp(buffer[0], TELEPORT, strlen(TELEPORT) + 1) == 0 && num_tokens == 3) {
      //printf("TELEPORT\n");
      ivec2 pos = { atoi(buffer[1]), atoi(buffer[2]) };
      //printf("pos: %d | %d\n", pos[0], pos[1]);
      //printf("Buffer[1]: %s | buffer[2]: %s\n", buffer[1], buffer[2]);
      teleport(pos);
    } else if (strncmp(buffer[0], SPAWN_ENEMY, strlen(SPAWN_ENEMY) + 1) == 0) {
      printf("SPAWN_ENEMY\n");
    } else if (strncmp(buffer[0], SPAWN_TRADE_SHIP, strlen(SPAWN_TRADE_SHIP) + 1) == 0) {
      printf("SPAWN_TRADE_SHIP\n");
    } else if (strncmp(buffer[0], SET_SPEED, strlen(SET_SPEED) + 1) == 0 && num_tokens == 2) {
      printf("SPEED RESET\n");
      float speed = atof(buffer[1]);
      set_speed(speed);
    } else {
      printf("%s: command not found\nEOW CONSOLE $ ", cmd);
      //printf("cmd: %s | cmd_len: %d\n", cmd, cmd_len);
    }
    fflush(stdout);
  }
}

void teleport(ivec2 pos) {
  vec2 coords = { pos[0], pos[1] };
  if (mode == EXPLORATION) {
    if (e_player.embarked) {
      world_to_chunk(coords, e_player.ship_chunk, e_player.ship_coords);
    } else {
      world_to_chunk(coords, e_player.chunk, e_player.coords);
    }
  } else {
    glm_vec2_copy(coords, c_player.coords);
  }
}

void set_speed(float speed) {
  if (mode == EXPLORATION) {
    e_player.speed = speed;
  } else if (mode == COMBAT) {
    c_player.speed = speed;
  }
}
