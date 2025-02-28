#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INST_LENGTH   30
#define MAX_ID_SIZE       5
#define MAX_WIRES_QT      1024
#define MAX_INST_CAPACITY 400

typedef enum {
  false = 0,
  true,
} Bool;

typedef struct {
  char *inst;
  Bool is_resolved;
} Inst;

typedef struct {
  Inst insts[MAX_INST_CAPACITY];
  size_t inst_size;
} Inst_List;

Inst_List il = {0};

void alloc_insts(void)
{
  for (size_t i = 0; i < MAX_INST_CAPACITY; ++i) {
    il.insts[i].inst = (char *) malloc(sizeof(char) * MAX_INST_LENGTH);
  }
}

void add_inst(char *inst)
{
  assert(il.inst_size < MAX_INST_CAPACITY);
  memcpy(il.insts[il.inst_size++].inst, inst, strlen(inst));
}

void free_insts(void)
{
  for (size_t i = 0; i < MAX_INST_CAPACITY; ++i) free(il.insts[i].inst);
}

typedef struct {
  char *id;
  uint16_t signal;
} Wire;

typedef struct {
  Wire wires[MAX_WIRES_QT];
  size_t wire_qt; // wire quantity
} Wire_Bundle;

Wire_Bundle wb = {0};

void alloc_wires(void)
{
  for (size_t i = 0; i < MAX_WIRES_QT; ++i) {
    wb.wires[i].id = (char *) malloc(sizeof(char) * MAX_ID_SIZE);
  }
}

void add_wire(char *id, uint16_t signal)
{
  assert(wb.wire_qt < MAX_WIRES_QT);
  memcpy(wb.wires[wb.wire_qt].id, id, strlen(id));
  wb.wires[wb.wire_qt++].signal = signal;
}

void print_wires(void)
{
  for (size_t i = 0; i < wb.wire_qt; ++i) {
    printf("id: %s\tsignal: %d\n", wb.wires[i].id, wb.wires[i].signal);
  }
}

void free_wires(void)
{
  for (size_t i = 0; i < MAX_WIRES_QT; ++i) free(wb.wires[i].id);
}

Bool is_id_present(char *id)
{
  for (size_t i = 0; i < wb.wire_qt; ++i) {
    if (strcmp(wb.wires[i].id, id) == 0) return 1;
  }
  return 0;
}

uint16_t get_wire_signal(char *id)
{
  for (size_t i = 0; i < wb.wire_qt; ++i) {
    if (strcmp(wb.wires[i].id, id) == 0) return wb.wires[i].signal;
  }
  printf("could not find signal for key %s\n", id);
  return 0;
}

void clear_inst(char *tmp_inst)
{
  for (size_t i = 0; i < MAX_INST_LENGTH; ++i) tmp_inst[i] = 0;
}

char *shift_by_delim(char **text, char delim)
{
  size_t index = 0;
  while (*(*text+index) != delim) index += 1;

  char *result = (char *) malloc(sizeof(char) * index + 1);
  result[index] = 0;
  memcpy(result, *text, index);
  
  *text += ++index;
  return result;
}

int is_numeric(char *digits)
{
  for (size_t i = 0; i < strlen(digits); ++i) {
    if (!('0' <= digits[i] && digits[i] <= '9')) return 0;
  }
  return 1;
}

#define SHIFT(text) (shift_by_delim(&(text), ' '))

Bool parse_inst(char *inst)
{
  uint16_t wire_signal;
  
  char *str1 = SHIFT(inst);
  char *str2 = SHIFT(inst);
  if (strcmp(str1, "NOT") == 0) {
    if (is_id_present(str2)) wire_signal = ~(get_wire_signal(str2));
    else return 0;
    if (strcmp(SHIFT(inst), "->") == 0) goto asd;
  } else {
    if (strcmp(str2, "->") == 0) {
      if (is_numeric(str1)) wire_signal = atoi(str1);
      else if (is_id_present(str1)) wire_signal = get_wire_signal(str1);
      else return 0;
      goto asd;
    } else {
      char *str3 = SHIFT(inst);
      
      uint16_t signal_1;
      uint16_t signal_2;
      
      if (is_numeric(str1)) signal_1 = atoi(str1);
      else if (is_id_present(str1)) signal_1 = get_wire_signal(str1);
      else return 0;
      
      if (is_numeric(str3)) signal_2 = atoi(str3);
      
      if ((is_id_present(str1) || is_numeric(str1)) && (is_id_present(str3) || is_numeric(str3))) {
	if (strcmp(str2, "AND") == 0 || strcmp(str2, "OR") == 0) {
	  if (is_id_present(str3)) signal_2 = get_wire_signal(str3);
	  else return 0;
	  
	  if (strcmp(str2, "AND") == 0) wire_signal = signal_1 & signal_2;
	  else if (strcmp(str2, "OR") == 0) wire_signal = signal_1 | signal_2;
	} else if (strcmp(str2, "LSHIFT") == 0 || strcmp(str2, "RSHIFT") == 0) {
	  if (strcmp(str2, "LSHIFT") == 0) wire_signal = signal_1 << signal_2;
	  else if (strcmp(str2, "RSHIFT") == 0) wire_signal = signal_1 >> signal_2;
	}
      } else {
	return 0;
      }
      if (strcmp(SHIFT(inst), "->") == 0) goto asd;
    }
  }

 asd:
  char *wire_id = SHIFT(inst);
  add_wire(wire_id, wire_signal);
  return 1;
}

Bool is_all_inst_resolved(void)
{
  size_t counter = 0;
  for (size_t i = 0; i < il.inst_size; ++i) {
    if (il.insts[i].is_resolved == true) counter += 1;
  }
  return (Bool) (counter == il.inst_size);
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
  char *inst = (char *) malloc(sizeof(char) * MAX_INST_LENGTH);
  clear_inst(inst);
  size_t index = 0;
  alloc_wires();
  alloc_insts();

  while ((ch = fgetc(file)) != EOF) {
    if (ch != '\n') {
      inst[index++] = ch;
    } else {
      index = 0;
      add_inst(inst);
      clear_inst(inst);
    }
  }

  while (is_all_inst_resolved() == false) {
    for (size_t i = 0; i < il.inst_size; ++i) {
      if (il.insts[i].is_resolved == false) {
	if (parse_inst(il.insts[i].inst)) il.insts[i].is_resolved = true;
      }
    }
  }

  // print_wires();
  printf("Signal to wire a: %d\n", get_wire_signal("a"));

  free_insts();
  free_wires();
  fclose(file);
  
  return 0;
}
