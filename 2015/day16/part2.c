#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STR_LENGTH 100
#define MAX_SUE_COUNT 500
#define MFCSAM_COMPOUNDS 10

typedef struct {
  int number;
  int compounds[MFCSAM_COMPOUNDS];
} Sue;

typedef struct {
  Sue sue_list[MAX_SUE_COUNT];
  size_t sue_count;
} Sue_List;

Sue_List sl = {0};

void reset_str(char *str)
{
  for (size_t i = 0; i < MAX_STR_LENGTH; ++i) str[i] = 0;
}

char *shift_by_delim(char **text, char delim)
{
  size_t index = 0;
  while ((*(*text+index) != delim) && (*(*text+index) != 0)) index += 1;

  char *result = (char *) malloc(sizeof(char) * index + 1);
  result[index] = 0;
  memcpy(result, *text, index);
  
  *text += ++index;
  return result;
}

#define SHIFT(text) (shift_by_delim(&(text), ' '))

void add_compound(char *cmp, int qt)
{
  size_t index;
  if (strcmp(cmp, "children") == 0)		index = 0;
  else if (strcmp(cmp, "cats") == 0)		index = 1;
  else if (strcmp(cmp, "samoyeds") == 0)	index = 2;
  else if (strcmp(cmp, "pomeranians") == 0)	index = 3;
  else if (strcmp(cmp, "akitas") == 0)		index = 4;
  else if (strcmp(cmp, "vizslas") == 0)		index = 5;
  else if (strcmp(cmp, "goldfish") == 0)	index = 6;
  else if (strcmp(cmp, "trees") == 0)		index = 7;
  else if (strcmp(cmp, "cars") == 0)		index = 8;
  else if (strcmp(cmp, "perfumes") == 0)	index = 9;
  
  sl.sue_list[sl.sue_count].compounds[index] = qt;
}

void add_sue(char *str)
{
  for (size_t i = 0; i < MFCSAM_COMPOUNDS; ++i) sl.sue_list[sl.sue_count].compounds[i] = -1;
  
  SHIFT(str); // Sue
  char *number = SHIFT(str);
  number = shift_by_delim(&number, ':');

  while (strlen(str) > 0) {
    char *cmp = SHIFT(str);
    cmp = shift_by_delim(&cmp, ':');
    char *qt = SHIFT(str);
    qt = shift_by_delim(&qt, ',');
    add_compound(cmp, atoi(qt));
  }
  
  sl.sue_list[sl.sue_count++].number = atoi(number);
}

void print_sue_list(void)
{
  for (size_t i = 0; i < sl.sue_count; ++i) {
    printf("Number: %d\t", sl.sue_list[i].number);
    for (size_t j = 0; j < MFCSAM_COMPOUNDS; ++j) printf("%2d\t", sl.sue_list[i].compounds[j]);
    printf("\n");
  }
}

int are_sues_equal(Sue *sue1, Sue *sue2)
{
  for (size_t i = 0; i < MFCSAM_COMPOUNDS; ++i) {
    if ((sue1->compounds[i] != -1) && (sue2->compounds[i] != -1)) {
      if (i == 1 || i == 7) { // cats or trees
	if (sue1->compounds[i] >= sue2->compounds[i]) return 0;
      } else if (i == 3 || i == 6) { // pomeranians or goldfish 
	if (sue1->compounds[i] <= sue2->compounds[i]) return 0;
      } else if (sue1->compounds[i] != sue2->compounds[i]) return 0;
    }
  }
  return 1;
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
  reset_str(str);
  size_t index = 0;

  while ((ch = fgetc(file)) != EOF) {
    if (ch != '\n') {
    str[index++] = ch;
    } else {
      index = 0;
      add_sue(str);
      reset_str(str);
    }
  }

  Sue target_sue = (Sue) {
    .number = 0,
    .compounds = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1},
  };

  for (size_t i = 0; i < MAX_SUE_COUNT; ++i) {
    if (are_sues_equal(&target_sue, &sl.sue_list[i])) printf("Number of Real Sue: %zu\n", i+1);
  }

  fclose(file);
  
  return 0;
}
