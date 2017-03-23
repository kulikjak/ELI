#include <stdio.h>

#include "input.h"
#include "eli.h"

#define BUFFER_SIZE 1024

// Global variables.
char line[BUFFER_SIZE];
char *linePointer = line;
FILE *inputFile;

// Load given input file and returns its size in bytes.
// In case of error program is terminated with EXIT_FAILURE value.
// File pointer is saved into global variable.
int initInput(const char* fileName) {
  int size;

  if ( !fileName )
    error("No input file.");

  inputFile = fopen(fileName, "rt");
  if ( !inputFile )
    error("Cannot open given file.");

  // get the file size
  fseek(inputFile, 0, SEEK_END);
  size = ftell(inputFile);
  fseek(inputFile, 0, SEEK_SET);

  return size;
}

// Read one character from the input file.
// Returns character of EOF (in case of EOF or error)
int getChar() {
  if (!*linePointer) {
    if (!fgets(line, BUFFER_SIZE, inputFile)) {
      fclose(inputFile);
      return EOF;
    }
    linePointer = line;
  }
  return *linePointer++;
}

// Reset file pointer to the beginning of the given file.
void resetFilePtr(void) {
  *linePointer = 0;
  fseek(inputFile, 0, SEEK_SET);
}
