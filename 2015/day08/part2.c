#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STR_LENGTH 50

#define FOR(i, value) for (size_t (i) = 0; (i) < (value); ++(i))

size_t get_num_of_chars_in_str_literal(const char *str)
{
  size_t counter = 0;
  FOR(i, strlen(str)) {
    if ((str[i] == '\\') || (str[i] == '"'))	counter += 2;
    else					counter += 1;
  }
  return counter+2;
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
  size_t index = 0;
  size_t counter = 0;

  while ((ch = fgetc(file)) != EOF) {
    if (ch != '\n') {
      str[index++] = ch;
    } else {
      index = 0;
      counter += (get_num_of_chars_in_str_literal(str) - strlen(str));
      FOR(i, MAX_STR_LENGTH) str[i] = 0;
    }
  }

  printf("Difference between actual chars and memory chars: %zu\n", counter);

  fclose(file);
  
  return 0;
}
