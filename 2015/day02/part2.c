#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void ssort(int *arr, size_t arr_size)
{
  for (size_t i = 0; i < arr_size; ++i) {
    int min = i;
    for (size_t j = i; j < arr_size; ++j) {
      if (arr[min] > arr[j]) min = j;
    }
    swap(&arr[i], &arr[min]);
  }
}

unsigned int get_ribbon_length(int *nums)
{
  ssort(nums, 3);
  return 2*(nums[0] + nums[1]) + nums[0]*nums[1]*nums[2];
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
  size_t index = 0;
  unsigned int ribbon_req = 0;
  int line = 0;
  
  while ((ch = fgetc(file)) != EOF) {    
    if ('0' <= ch && ch <= '9') {
      nums[index] = nums[index] * 10 + ch - '0';
    } else if (ch == 'x') {
      index += 1;
    } else if (ch == '\n') {
      ribbon_req += get_ribbon_length(nums);
   
      index = 0;
      line += 1;
      nums[0] = nums[1] = nums[2] = 0;
    }
  }

  printf("%d\n", line);
  printf("%d\n", ribbon_req);

      
  fclose(file);
  
  return 0;
}
