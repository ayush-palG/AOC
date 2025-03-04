#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void naive(size_t target)
{
  size_t value = 0;
  size_t i, j;
  for (i = 1; value < target; i++) {
    value = 0;
    for (j = 1; j <= i; ++j) {
      if (i % j == 0) value += j;
    }
  }
  printf("%zu -> %zu\n", i-1, value*10);
}

void sieve(size_t *arr, size_t target)
{
  for (size_t i = 1; i <= target; ++i) {
    for (size_t j = i; j <= target; j += i) {
      arr[j] += i;
    }
  }

  for (size_t i = 1; i < target; ++i) {
    if (arr[i] >= target) {
      printf("%zu -> %zu\n", i, arr[i]*10);
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

  target /= 10;
  size_t *arr = (size_t *) malloc(sizeof(size_t) * target);
  // naive(target);
  sieve(arr, target);
  
  fclose(file);
  
  return 0;
}
