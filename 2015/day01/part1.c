#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(void)
{
  const char *input_file_path = "input";
  FILE *file = fopen(input_file_path, "r");

  if (file == NULL) {
    fprintf(stderr, "ERROR: could not open file %s: %s\n", input_file_path, strerror(errno));
    exit(1);
  }

  char ch;
  int floor_pos = 0;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '(') {
      floor_pos += 1;
    } else if (ch == ')') {
      floor_pos -= 1;
    }
  }

  fclose(file);
  
  printf("Floor Position: %d\n", floor_pos);
  
  return 0;
}
