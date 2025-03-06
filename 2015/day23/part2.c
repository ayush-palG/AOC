#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define MAX_STR_LENGTH  10
#define MAX_INST_COUNT  50
#define REGISTERS_COUNT 2

typedef enum {
  REG_A = 0,
  REG_B = 1,
} Reg;

typedef enum {
  INST_HLF,
  INST_TPL,
  INST_INC,
  INST_JMP,
  INST_JIE,
  INST_JIO,
} Inst_Type;

typedef struct {
  Inst_Type type;
  Reg reg;
  int offset;
} Inst;

void reset_str(char *str)
{
  for (size_t i = 0; i < MAX_STR_LENGTH; ++i) str[i] = 0;
}

typedef struct {
  Inst insts[MAX_INST_COUNT];
  size_t insts_count;
  size_t inst_pointer;
} Inst_List;


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

#define SHIFT(str) shift_by_delim(&(str), ' ')

Reg cstr_to_reg(char *cstr)
{
  if (strcmp(cstr, "a") == 0) return REG_A;
  else return REG_B;
}

void push_inst(char *str, Inst_List *il)
{
  char *inst_str = SHIFT(str);
  char *reg_str = SHIFT(str);
  Inst inst = {0};
  
  if (strcmp(inst_str, "hlf") == 0) {
    inst = (Inst) {.type = INST_HLF, .reg = cstr_to_reg(reg_str)};
  } else if (strcmp(inst_str, "tpl") == 0) {
    inst = (Inst) {.type = INST_TPL, .reg = cstr_to_reg(reg_str)};
  } else if (strcmp(inst_str, "inc") == 0) {
    inst = (Inst) {.type = INST_INC, .reg = cstr_to_reg(reg_str)};    
  } else if (strcmp(inst_str, "jmp") == 0) {
    char *offset = reg_str;
    inst = (Inst) {.type = INST_JMP, .offset = atoi(offset)};
  } else if (strcmp(inst_str, "jie") == 0) {
    reg_str = shift_by_delim(&reg_str, ',');
    char *offset = SHIFT(str);
    inst = (Inst) {.type = INST_JIE, .reg = cstr_to_reg(reg_str), .offset = atoi(offset)};
  } else if (strcmp(inst_str, "jio") == 0) {
    reg_str = shift_by_delim(&reg_str, ',');
    char *offset = SHIFT(str);
    inst = (Inst) {.type = INST_JIO, .reg = cstr_to_reg(reg_str), .offset = atoi(offset)};
  }

  assert(il->insts_count < MAX_INST_COUNT-1);
  il->insts[il->insts_count++] = inst;
}

char *inst_as_cstr(Inst inst)
{
  switch(inst.type) {
  case INST_HLF: return "INST_HLF";
  case INST_TPL: return "INST_TPL";
  case INST_INC: return "INST_INC";
  case INST_JMP: return "INST_JMP";
  case INST_JIE: return "INST_JIE";
  case INST_JIO: return "INST_JIO";
  default: return "inst_as_cstr: unreachable";
  }
}

void execute_inst(Inst_List *il, long int *regs)
{
  if (il->inst_pointer >= il->insts_count) return;

  Inst inst = il->insts[il->inst_pointer];

  switch (inst.type) {
  case INST_HLF:
    regs[inst.reg] /= 2;
    il->inst_pointer += 1;
    break;
  case INST_TPL:
    regs[inst.reg] *= 3;
    il->inst_pointer += 1;
    break;
  case INST_INC:
    regs[inst.reg] += 1;
    il->inst_pointer += 1;
    break;
  case INST_JMP:
    il->inst_pointer += inst.offset;
    break;
  case INST_JIE:
    if (regs[inst.reg] % 2 == 0) il->inst_pointer += inst.offset;
    else il->inst_pointer += 1;
    break;
  case INST_JIO:
    if (regs[inst.reg] == 1) il->inst_pointer += inst.offset;
    else il->inst_pointer += 1;
    break;
  }
}

long int regs[REGISTERS_COUNT] = {1, 0};
Inst_List il = {0};

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
      push_inst(str, &il);
      reset_str(str);
    }
  }

  while (il.inst_pointer < il.insts_count) {
    execute_inst(&il, regs);
  }
  
  for (size_t i = 0; i < REGISTERS_COUNT; ++i) {
    printf("%c: %ld\n", (char) ('a'+i), regs[i]);
  }

  fclose(file);
  
  return 0;
}
