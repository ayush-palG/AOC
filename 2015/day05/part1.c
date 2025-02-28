#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

int is_string_nice(const char *str)
{
  size_t str_length = strlen(str);
  const char vowels[] = {'a', 'e', 'i', 'o', 'u', '\0'};
  size_t vowels_count = 0;
  size_t letters_appeared_twice = 0;
  const char *disallowed_substr[] = {"ab", "cd", "pq", "xy"};
  size_t disallowed_substr_in_str = 0;
  
  for (size_t i = 0; i < str_length; ++i) {
    for (size_t j = 0; j < strlen(vowels); ++j) {
      if (str[i] == vowels[j]) vowels_count++;
    }
  }

  for (size_t i = 0; i < str_length-1; ++i) {
    if (str[i] == str[i+1]) {
      letters_appeared_twice = 1;
      break;
    }
  }

  /*
    Here, I allocated memory for sub_arr using malloc(), so I have to manually
    insert a NULL character at last in the sub_arr or else get the wrong results
  */
  size_t substr_length = 2;
  char *sub_arr = (char *) malloc(sizeof(char) * substr_length + 1);
  sub_arr[substr_length] = 0;
  for (size_t i = 0; i < (str_length-substr_length+1); ++i) {
    memcpy(sub_arr, &str[i], substr_length);
    for (size_t j = 0; j < 4; ++j) {
      if (strcmp(sub_arr, disallowed_substr[j]) == 0) {
	disallowed_substr_in_str = 1;
	break;
      }
    }
    if (disallowed_substr_in_str) break;
  }
  
  free(sub_arr);

  // printf("%zu\t%zu\t%d\n", vowels_count, letters_appeared_twice, !disallowed_substr_in_str);
  return (vowels_count >= 3) && (letters_appeared_twice) && (!disallowed_substr_in_str);
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
    if (ch == '\n' && is_string_nice(string)) nice_string_count += 1;
    
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
