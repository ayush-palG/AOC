#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COORD_CAPACITY 1024*4

typedef struct {
  int x, y;
} Coord;

typedef struct {
  Coord *coord;
  size_t size;
} Coord_List;

Coord_List cl = {0};

void initialize_coord(Coord_List *cl)
{
  cl->coord = (Coord *) malloc(sizeof(Coord) * COORD_CAPACITY);

  for (size_t i = 0; i < COORD_CAPACITY; ++i) {
    cl->coord[i] = (Coord) {0};
  }

  cl->coord[0] = (Coord) {.x = 0, .y = 0};
  cl->size++;
}

int is_coord_eq(Coord a, Coord b)
{
  return a.x == b.x && a.y == b.y;
}

int is_gift_recieved(Coord_List *cl, Coord coord)
{
  for (size_t i = 0; i < cl->size; ++i) {
    if (is_coord_eq(cl->coord[i], coord)) return 1;
  }
  return 0;
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
  Coord curr_coord[2];
  initialize_coord(&cl);
  size_t counter = 0;
  size_t index = 0;
  
  while ((ch = fgetc(file)) != EOF) {
    index = counter % 2;
    if (ch == '^') {
      curr_coord[index].x += 1;
    } else if (ch == '>') {
      curr_coord[index].y += 1;
    } else if (ch == 'v') {
      curr_coord[index].x -= 1;
    } else if (ch == '<') {
      curr_coord[index].y -= 1;
    }

    if (!is_gift_recieved(&cl, curr_coord[index])) cl.coord[cl.size++] = curr_coord[index];
    counter += 1;
  }

  printf("%zu\n", cl.size);
  
  fclose(file);
  
  return 0;
}

