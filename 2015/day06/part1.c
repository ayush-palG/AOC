#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

typedef enum {
  false = 0,
  true,
} Bool;

Bool lights_state[1000][1000];

/*
  turn
  on
  0,0
  through (could get rid of this)
  999,999
*/

void clear_command(char *cmd, size_t cmd_capacity)
{
  for (size_t i = 0; i < cmd_capacity; ++i) cmd[i] = 0;
}

size_t count_lights_state(void)
{
  size_t counter = 0;
  for (size_t i = 0; i < 1000; ++i) {
    for (size_t j = 0; j < 1000; ++j) {
      if (lights_state[i][j]) counter += 1;
    }
  }
  return counter;
}

void execute_light_action(size_t x1, size_t y1, size_t x2, size_t y2, char action)
{
  // t -> toggle, o -> open, c -> close
  
  for (size_t i = x1; i <= x2; ++i) {
    for (size_t j = y1; j <= y2; ++j) {
      switch (action) {
      case 't':
	if (lights_state[i][j]) lights_state[i][j] = 0;
	else lights_state[i][j] = 1;
	break;
      case 'o':
	lights_state[i][j] = 1;
	break;
      case 'c':
	lights_state[i][j] = 0;
	break;
      default: printf("could not execute action: %c\n", action);
	return;
      }
    }
  }
}

size_t size_t_atoi(const char *text)
{
  size_t result = 0;
  size_t text_size = strlen(text);
  for (size_t i = 0; i < text_size; ++i) {
    assert('0' <= text[i] && text[i] <= '9');
    result = result * 10 + text[i] - '0';
  }
  return result;
}

size_t get_delim_index(char *text, char delim)
{
  size_t index = 0;
  while (text[index] != delim) {
    index += 1;
  }
  return index;
}

char *shift(char **text, char delim)
{
  size_t space_index = get_delim_index(*text, delim);

  char *result = (char *) malloc(sizeof(char) * space_index + 1);
  result[space_index] = 0;
  memcpy(result, *text, space_index);
  
  *text += space_index+1;
  return result;
}

void execute_cmd(char *cmd, char action)
{
  char *subcmd;
  
  subcmd = shift(&cmd, ',');
  size_t x1 = size_t_atoi(subcmd);
  subcmd = shift(&cmd, ' ');
  size_t y1 = size_t_atoi(subcmd);
  subcmd = shift(&cmd, ' ');
  subcmd = shift(&cmd, ',');
  size_t x2 = size_t_atoi(subcmd);
  subcmd = shift(&cmd, ' ');
  size_t y2 = size_t_atoi(subcmd);
  execute_light_action(x1, y1, x2, y2, action);
}

void parse_command(char *cmd)
{
  char *subcmd;

  subcmd = shift(&cmd, ' ');
  if (strcmp(subcmd, "turn") == 0) {
    subcmd = shift(&cmd, ' ');
    if (strcmp(subcmd, "on") == 0) {
      execute_cmd(cmd, 'o');
    } else if (strcmp(subcmd, "off") == 0) {
      execute_cmd(cmd, 'c');
    }
  } else if (strcmp(subcmd, "toggle") == 0) {
    execute_cmd(cmd, 't');
  }
}

int main(void)
{
  const char *input_file_path = "input";
  FILE *file = fopen(input_file_path, "r");
  if (file == NULL) {
    fprintf(stderr, "ERROR: could not open file %s: %s\n", input_file_path, strerror(errno));
    exit(1);
  }

  char ch;
  char *command = (char *) malloc(sizeof(char) * 10*20);
  clear_command(command, 10*20);
  size_t index = 0;
  
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') {
      index = 0;
      parse_command(command);
      clear_command(command, 10*20);
    } else {
      command[index++] = ch;
    }
  }

  printf("Number of lights turn on: %zu\n", count_lights_state());

  free(command);
  fclose(file);
  
  return 0;
}
