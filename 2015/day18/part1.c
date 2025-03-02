#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STR_LENGTH 10
#define TEST_GRID_SIZE 6
#define TEST_STEPS_COUNT 4
#define GRID_SIZE 100
#define STEPS_COUNT 100

#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs)[0]))

typedef enum {
  false = 0,
  true,
} Bool;

Bool lights[GRID_SIZE*GRID_SIZE];
Bool tmp_lights[GRID_SIZE*GRID_SIZE];

size_t count_open_lights(Bool *lights)
{
  size_t counter = 0;
  for (size_t i = 0; i < GRID_SIZE; ++i) {
    for (size_t j = 0; j < GRID_SIZE; ++j) {
      if (lights[i*GRID_SIZE + j] == true) counter += 1;
    }
  }
  return counter;
}

Bool is_valid_pos(size_t pos, size_t grid_num)
{
  return (pos < GRID_SIZE*GRID_SIZE) && (pos / GRID_SIZE == grid_num);
}

size_t count_neighbour_lights(Bool *lights, size_t pos)
{
  size_t counter = 0;
  size_t pos_grid_num = pos / GRID_SIZE;
  if ((is_valid_pos(pos-GRID_SIZE-1, pos_grid_num-1)) && (lights[pos-GRID_SIZE-1] == true)) counter += 1;
  if ((is_valid_pos(pos-GRID_SIZE, pos_grid_num-1)) && (lights[pos-GRID_SIZE] == true)) counter += 1;
  if ((is_valid_pos(pos-GRID_SIZE+1, pos_grid_num-1)) && (lights[pos-GRID_SIZE+1] == true)) counter += 1;
  if ((is_valid_pos(pos-1, pos_grid_num)) && (lights[pos-1] == true)) counter += 1;
  if ((is_valid_pos(pos+1, pos_grid_num)) && (lights[pos+1] == true)) counter += 1;
  if ((is_valid_pos(pos+GRID_SIZE-1, pos_grid_num+1)) && (lights[pos+GRID_SIZE-1] == true)) counter += 1;
  if ((is_valid_pos(pos+GRID_SIZE, pos_grid_num+1)) && (lights[pos+GRID_SIZE] == true)) counter += 1;
  if ((is_valid_pos(pos+GRID_SIZE+1, pos_grid_num+1)) && (lights[pos+GRID_SIZE+1] == true)) counter += 1;
  return counter;
}

void print_lights(Bool *lights)
{
  for (size_t i = 0; i < GRID_SIZE; ++i) {
    for (size_t j = 0; j < GRID_SIZE; ++j) {
      if (lights[i*GRID_SIZE + j] == true) printf("#");
      else printf(".");
    }
    printf("\n");
  }
}

void next_stage(Bool *tmp_lights, Bool *lights)
{
  memcpy(tmp_lights, lights, sizeof(Bool) * GRID_SIZE*GRID_SIZE);
  for (size_t i = 0; i < GRID_SIZE; ++i) {
    for (size_t j = 0; j < GRID_SIZE; ++j) {
      if (lights[i*GRID_SIZE + j] == true) {
	size_t open_lights = count_neighbour_lights(lights, i*GRID_SIZE+j);
	if (!(open_lights == 2 || open_lights == 3)) tmp_lights[i*GRID_SIZE + j] = false;
      } else if (lights[i*GRID_SIZE + j] == false) {
	size_t close_lights  = count_neighbour_lights(lights, i*GRID_SIZE+j);
	if (close_lights == 3) tmp_lights[i*GRID_SIZE + j] = true;
      }
    }
  }
  memcpy(lights, tmp_lights, sizeof(Bool) * GRID_SIZE*GRID_SIZE);
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
  size_t index = 0;

  while ((ch = fgetc(file)) != EOF) {
    if (ch == '#')      lights[index++] = true;
    else if (ch == '.') lights[index++] = false;
  }

  for (size_t i = 0; i < STEPS_COUNT; ++i) next_stage(tmp_lights, lights);
  printf("After %d step, the open lights count is: %zu\n", STEPS_COUNT, count_open_lights(lights));

  fclose(file);
  
  return 0;
}
