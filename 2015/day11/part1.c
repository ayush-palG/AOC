#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STR_LENGTH 30

void reset_str(char *str)
{
  for (size_t i = 0; i < MAX_STR_LENGTH; ++i) str[i] = 0;
}

int is_pass_valid(char *pass)
{
  size_t consecutive_three_letters = 0;
  size_t contains_iol = 0;
  size_t contains_two_diff_pairs_of_letters = 0;

  for (size_t i = 0; i < strlen(pass)-2; ++i) {
    if (((pass[i]) == (pass[i+1]-1)) && ((pass[i]) == (pass[i+2]-2))) consecutive_three_letters = 1;
  }

  for (size_t i = 0; i < strlen(pass); ++i) {
    if ((pass[i] == 'i') || (pass[i] == 'o') || (pass[i] == 'l')) contains_iol = 1;
  }

  char ch = 0;
  for (size_t i = 0; i < strlen(pass)-1; ++i) {
    if ((pass[i] == pass[i+1]) && (ch != pass[i])) {
      ch = pass[i++];
      contains_two_diff_pairs_of_letters += 1;
    }
  }

  return (consecutive_three_letters) && (!(contains_iol)) && (contains_two_diff_pairs_of_letters >= 2);
}

void incr_str(char *str)
{
  size_t index = strlen(str) - 1;

  str[index] += 1;
  while (str[index] > 'z') {
    str[index--] = 'a';
    str[index] += 1;
  }
}

void print_next_pass(char *curr_pass)
{
  while (!(is_pass_valid(curr_pass))) incr_str(curr_pass);
  printf("%s\n", curr_pass);
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
  char *curr_pass = (char *) malloc(sizeof(char) * MAX_STR_LENGTH);
  reset_str(curr_pass);
  size_t index = 0;

  while ((ch = fgetc(file)) != EOF) curr_pass[index++] = ch;

  print_next_pass(curr_pass);
  fclose(file);
  
  return 0;
}
