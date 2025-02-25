#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int min3(int a, int b, int c)
{
  return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c) ;
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
  int nums[3] = {0};
  int lbh_nums[3] = {0};
  size_t index = 0;
  size_t wrapping_paper_req = 0;
  
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') {
      lbh_nums[0] = nums[0] * nums[1];
      lbh_nums[1] = nums[0] * nums[2];
      lbh_nums[2] = nums[1] * nums[2];
      wrapping_paper_req += 2 * (lbh_nums[0] + lbh_nums[1] + lbh_nums[2]) + min3(lbh_nums[0], lbh_nums[1], lbh_nums[2]);

      index = 0;
      nums[0] = nums[1] = nums[2] = 0;
      lbh_nums[0] = lbh_nums[1] = lbh_nums[2] = 0;
      continue;
    }
    
    if ('0' <= ch && ch <= '9') {
      nums[index] = nums[index] * 10 + ch - '0';
    } else if (ch == 'x') {
      index += 1;
    }
  }

  lbh_nums[0] = nums[0] * nums[1];
  lbh_nums[1] = nums[0] * nums[2];
  lbh_nums[2] = nums[1] * nums[2];
  wrapping_paper_req += 2 * (lbh_nums[0] + lbh_nums[1] + lbh_nums[2]) + min3(lbh_nums[0], lbh_nums[1], lbh_nums[2]);

  printf("%3d %3d %3d\n", nums[0], nums[1], nums[2]);
  printf("%zu\n", wrapping_paper_req);
  
  return 0;
}
