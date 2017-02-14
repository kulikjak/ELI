#include <stdio.h>
#include <stdlib.h>

#include "eli.h"

// Global registers
bool r_memory_dump = false;
bool r_instr_dump = false;
bool r_lang_dump = false;

// Print error message and immediately terminate the program.
void error(char * a) {
  printf("ERROR: %s\n", a);
  exit(EXIT_FAILURE);
}

// Array of user readable instruction names.
const char *InstructionTable[9] = {
  "INC", "DEC", "PRINTC", "SCANC", "FWD", "BWD", "BRZ", "BNZ", "EXIT"
};
