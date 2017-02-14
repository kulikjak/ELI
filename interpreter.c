#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "eli.h"
#include "parser.h"

#define MAXIMUM_STACK_SIZE 128 // Maximum depth of nested conditions.
#define MAXIMUM_ARRAY_SIZE 200 // Size of the main memory.

// Stack memory.
Instruction* stack[MAXIMUM_STACK_SIZE];

// Current size of the stack & first unused stack field.
int s_top = 0;

// Push instruction address onto the stack.
// Push, Pop and Top functions are used for conditional statements and loops.
void push(Instruction* addr) {
  if ((s_top+1) >= MAXIMUM_STACK_SIZE) 
    error("Stack overflow.");
  stack[s_top++] = addr;
} 

// Remove one address from the stack.
void pop() {
  if (s_top <= 0)
    error("Stack underflow.");
  s_top--;
}

// Get instruction address from the stack.
Instruction* top() {
  return stack[s_top-1];
}

// Skip whole block of code after unsatisfied condition.
// Function can terminate program if unexpected EOF is inside the loop block.
Instruction* skip(Instruction* inst) {
  int depth = 0;
  while(*inst != EXIT) {
    switch(*inst) {
      case BRZ:
        depth++;
        break;
      case BNZ:
        depth--;
        if (!depth)
          return inst;
      default:
        break;
    }
    inst++;
  }
  error("Unexpected end of program.");
  return NULL; // not reachable
}

// Dump content of the memory to stdout.
void dump_memory(int *array) {
  int i, j;

  printf("\n  Mem  |");
  for (i = 0; i < 30; i++)
    printf("%3d ", i);
  printf("\n-------|");
  for (i = 0; i < 30; i++)
    printf("----");
  printf("\n");
  for (i = 0; i < MAXIMUM_ARRAY_SIZE; i+= 30) {
    printf("%6d |", i);
    for (j = 0; j < 30 && j+i < MAXIMUM_ARRAY_SIZE; j++) 
      printf("%3d ", array[i+j]);
    printf("\n");
  } 
}

// Dump sequence of instructions to stdout.
void dump_instructions(Instruction* inst) {
  int i;

  printf("Instruction dump:\n");
  for(i = 0; *inst != EXIT; i++, inst++) {
    printf("%s ", InstructionTable[*inst]);
    if (i == 30) {
      i = 0;
      printf("\n");
    }
  }
  printf("\n\n");
}

// Run the virtual machine.
void run (Instruction* inst) {
  int array[MAXIMUM_ARRAY_SIZE];
  int ptr = 0;

  memset(array, 0, MAXIMUM_ARRAY_SIZE*sizeof(int)); 

  while (*inst != EXIT) {     
    switch(*inst) {
      case INC:
        array[ptr] ++;
        break;
      case DEC:
        array[ptr] --;
        break;
      case FWD:
        ptr ++;   
        break;
      case BWD:
        ptr --;
        break;
      case PRINTC:
        putchar(array[ptr]);
        break;
      case SCANC:
        array[ptr] = getchar();
        break;
      case BRZ:
        if (array[ptr]) push(inst);
        else inst = skip(inst);
        break;
      case BNZ: // skips backwards
        if (array[ptr]) inst = top();
        else pop();
        break;
      default:
        error("Unknown instruction!");
        break;
      }
    inst++;
  }

  if (r_memory_dump) 
    dump_memory((int *)&array);
}

// Print help block into stdout.
void help(char* name) {
  printf("Esoteric language interpreter (ELI)\n");
  printf("Usage:\n");
  printf("    %s -ilm [FILE]\n\n", name);
  printf("     i flag dumps used instructions.\n");
  printf("     l flag shows used language.\n");
  printf("     m flag dumps memory at the end of execution.\n");

  exit(EXIT_SUCCESS);
}

int main (int argc, char *argv[]) {
  char c;

  if (argc < 2) help(argv[0]);

  while ((c = getopt (argc, argv, "ilm")) != -1) {
    switch (c) {
      case 'i':
        r_instr_dump = true;
        break;
      case 'l':
        r_lang_dump = true;
        break;
      case 'm':
        r_memory_dump = true;
        break;
      default:
        printf("Unknown flag.\n");
        help(argv[0]);
        break;
    }
  }

  Instruction* inst = parseFile(argv[optind]);
  if (r_instr_dump)
    dump_instructions(inst);

  run(inst);
  free(inst);

  return EXIT_SUCCESS;
}
