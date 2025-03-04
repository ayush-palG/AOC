#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void sieve(size_t *arr, size_t target)
{
  for (size_t i = 1; i <= target; ++i) {
    for (size_t j = i; j <= target && j <= 50*i; j += i) {
      arr[j] += i*11;
    }
  }

  for (size_t i = 1; i < target; ++i) {
    if (arr[i] >= target) {
      printf("%zu -> %zu\n", i, arr[i]);
      break;
    }
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
  size_t target = 0;
  while ((ch = fgetc(file)) != EOF) target = target * 10 + ch - '0';

  size_t *arr = (size_t *) malloc(sizeof(size_t) * target);
  sieve(arr, target);
  
  fclose(file);
  
  return 0;
}
