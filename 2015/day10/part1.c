#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STR_LENGTH 1024*1024

void reset_str(char *str)
{
  for (size_t i = 0; i < MAX_STR_LENGTH-1; ++i) str[i] = 0;
}

void get_next_value(char *curr_value)
{
  char *next_value = (char *) malloc(sizeof(char) * MAX_STR_LENGTH);
  reset_str(next_value);
  size_t nv_index = 0;
  size_t counter = 0;
  
  for (size_t i = 0; i < strlen(curr_value); ++i) {
    if (curr_value[i] == curr_value[i+1]) {
      counter += 1;
    } else {
      next_value[nv_index++] = counter + '1';
      next_value[nv_index++] = curr_value[i];
      counter = 0;
    }
  }

  reset_str(curr_value);
  memcpy(curr_value, next_value, strlen(next_value));
  free(next_value);
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
  char *curr_value = (char *) malloc(sizeof(char) * MAX_STR_LENGTH);
  reset_str(curr_value);
  size_t index = 0;

  while ((ch = fgetc(file)) != EOF) curr_value[index++] = ch;

  for (size_t i = 0; i < 40; ++i) {
    get_next_value(curr_value);
    printf("%zu: %ld\n", i+1, strlen(curr_value));
  }
  
  fclose(file);
  
  return 0;
}
