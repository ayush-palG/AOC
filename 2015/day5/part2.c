#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

typedef enum {
  false = 0,
  true,
} Bool;

Bool is_string_nice(const char *str)
{
  Bool pair_letters_appeared_twice = 0;
  Bool one_in_two_letters = 0;
  size_t str_length = strlen(str);
  
  char curr_sub_arr[3] = {0};
  char moving_sub_arr[3] = {0};
  for (size_t i = 0; i < (str_length-2); ++i) {
    memcpy(curr_sub_arr, &str[i], 2);
    for (size_t j = i+2; j < str_length; ++j) {
      memcpy(moving_sub_arr, &str[j], 2);
      if (strcmp(curr_sub_arr, moving_sub_arr) == 0) {
	pair_letters_appeared_twice = 1;
	break;
      }
    }
    if (pair_letters_appeared_twice) break;
  }

  for (size_t i = 0; i < (str_length-2); ++i) {
    if (str[i] == str[i+2]) {
      one_in_two_letters = 1;
      break;
    }
  }

  // printf("%d\t%d\n", pair_letters_appeared_twice, one_in_two_letters);
  return pair_letters_appeared_twice && one_in_two_letters;
}

void init_string(char *string, size_t string_capacity)
{
  for (size_t i = 0; i < string_capacity; ++i) string[i] = 0;
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
  char *string = (char *) malloc(sizeof(char) * 20);
  size_t index = 0;
  size_t nice_string_count = 0;

  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n' && (is_string_nice(string) == true)) nice_string_count += 1;
    
    if (ch != '\n') {
      string[index++] = ch;
    } else {
      index = 0;
      init_string(string, 20);
    }
  }

  free(string);
  fclose(file);

  printf("Number of Nice strings: %zu\n", nice_string_count);
  
  return 0;
}
