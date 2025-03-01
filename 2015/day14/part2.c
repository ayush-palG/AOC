#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STR_LENGTH 10
#define MAX_NAME_LENGTH 10
#define MAX_REINDEER_COUNT 30

typedef struct {
  char *name;
  size_t speed;
  size_t fly_dur;
  size_t rest_dur;
  size_t dist;
  size_t points;
} Reindeer;

typedef struct {
  Reindeer reindeers[MAX_REINDEER_COUNT];
  size_t reindeers_count;
} Reindeers;

Reindeers rds = {0};

void alloc_reindeer_names(void)
{
  for (size_t i = 0; i < MAX_REINDEER_COUNT; ++i)
    rds.reindeers[i].name = (char *) calloc(sizeof(char), MAX_NAME_LENGTH);
}

void reset_str(char *str)
{
  for (size_t i = 0; i < MAX_STR_LENGTH; ++i) str[i] = 0;
}

char *shift_by_delim(char **text, char delim)
{
  size_t index = 0;
  while (*(*text+index) != delim) index += 1;

  char *result = (char *) malloc(sizeof(char) * index + 1);
  result[index] = 0;
  memcpy(result, *text, index);
  
  *text += ++index;
  return result;
}

size_t size_t_atoi(char *digits)
{
  size_t result = 0;
  for (size_t i = 0; i < strlen(digits); ++i) {
    if (!('0' <= digits[i] && digits[i] <= '9')) return 0;
    else result = result * 10 + digits[i] - '0';
  }
  return result;
}

#define SHIFT(text) (shift_by_delim(&(text), ' '))

void add_reindeer(char *str)
{
  char *name = SHIFT(str);
  SHIFT(str); // can
  SHIFT(str); // fly
  char *speed = SHIFT(str);
  SHIFT(str); // km/s
  SHIFT(str); // for
  char *fly_dur = SHIFT(str);
  SHIFT(str); // seconds,
  SHIFT(str); // but
  SHIFT(str); // then
  SHIFT(str); // must
  SHIFT(str); // rest
  SHIFT(str); // for
  char *rest_dur = SHIFT(str);

  rds.reindeers[rds.reindeers_count++] = (Reindeer) {
    .name = name,
    .speed = size_t_atoi(speed),
    .fly_dur = size_t_atoi(fly_dur),
    .rest_dur = size_t_atoi(rest_dur),
  };
}

void print_reindeer(Reindeer *rd)
{
  printf("Name: %s\nSpeed: %zu\nFly_Dur: %zu\nRest_Dur: %zu\nDist: %zu\nPoints: %zu\n\n", rd->name, rd->speed, rd->fly_dur, rd->rest_dur, rd->dist, rd->points);
}

void calc_dist_travelled(Reindeer *rd, size_t s_time, size_t e_time)
{
  size_t fly_rest_dur = rd->fly_dur + rd->rest_dur;

  for (size_t i = s_time; i < e_time; ++i) {
    if ((i % fly_rest_dur) < rd->fly_dur) rd->dist += rd->speed;
  }
}

void incr_pt_of_lead_rd(void)
{
  size_t max_dist = 0;
  for (size_t i = 0; i < rds.reindeers_count; ++i) {
    if (rds.reindeers[i].dist > max_dist) max_dist = rds.reindeers[i].dist;
  }
  for (size_t i = 0; i < rds.reindeers_count; ++i) {
    if (max_dist == rds.reindeers[i].dist) rds.reindeers[i].points += 1;
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
  char *str = (char *) malloc(sizeof(char) * MAX_STR_LENGTH);
  reset_str(str);
  size_t index = 0;
  alloc_reindeer_names();

  while ((ch = fgetc(file)) != EOF) {
    if (ch != '\n') {
    str[index++] = ch;
    } else {
      index = 0;
      add_reindeer(str);
      reset_str(str);
    }
  }

  size_t time = 2503;

  for (size_t i = 0; i < time; ++i) {
    for (size_t j = 0; j < rds.reindeers_count; ++j) calc_dist_travelled(&rds.reindeers[j], i, i+1);
    incr_pt_of_lead_rd();
  }

  size_t max_points = 0;
  for (size_t i = 0; i < rds.reindeers_count; ++i) {
    if (rds.reindeers[i].points > max_points) max_points = rds.reindeers[i].points;
  }
  
  printf("Winning Reindeer travelled: %zu\n", max_points);
  fclose(file);
  
  return 0;
}
