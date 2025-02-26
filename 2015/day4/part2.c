#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define MD5_IMPLEMENTATION
#include "stb_md5.h"

#define SECRET_KEY_CAPACITY 20

int is_leading_by_zeros(const char *text, size_t zeros_num)
{
  assert(strlen(text) >= zeros_num);
  for (size_t i = 0; i < zeros_num; ++i) {
    if (text[i] != '0') return 0;
  }
  return 1;
}

int main(void)
{
  const char *input = "iwrupvqb";
  char *secret_input = (char *) malloc(sizeof(char) * SECRET_KEY_CAPACITY + strlen(input));
  size_t counter = 0;
  const char *digest;

  do {
    sprintf(secret_input, "%s%zu", input, counter);
    digest = md5(secret_input, 'm');
    counter++;
  } while (is_leading_by_zeros(digest, 6) == 0);

  printf("%zu\n", --counter);
  
  return 0;
}
