#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "eli.h"
#include "parser.h"

// Tables of instruction symbols for each supported language.
const char brainfuck_symbols[8] = {'+', '-', '.', ',', '>', '<', '[', ']'};
const char ook_symbols_f[8] = {'.', '!', '!', '.', '.', '?', '!', '?'};
const char ook_symbols_s[8] = {'.', '!', '.', '!', '?', '.', '?', '!'};

// Generate Ook! code and send it into given file.
void generateOok(Instruction* inst, FILE* out) {

  while (*inst != EXIT) {
    fprintf(out, "Ook%c Ook%c ", ook_symbols_f[*inst], ook_symbols_s[*inst]);
    inst++;
  }
  fprintf(out, "\n");
}

// Generate compressed Ook! code and send it into given file.
void generateCompressedOok(Instruction* inst, FILE* out) {

  while (*inst != EXIT) {
    fprintf(out, "%c%c ", ook_symbols_f[*inst], ook_symbols_s[*inst]);
    inst++;
  }
  fprintf(out, "\n");
}

// Generate brainf*ck code and send it into given file.
void generateBrainfuck(Instruction* inst, FILE* out) {

  while (*inst != EXIT) {
    fprintf(out, "%c", brainfuck_symbols[*inst]);
    inst++;
  }
  fprintf(out, "\n");
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

// Print help block into stdout.
void help(char* name) {
  printf("Esoteric language interpreter - Transpiler (ELI/T)\n");
  printf("Usage:\n");
  printf("    %s -ilm [FILE]\n\n", name);
  printf("     i flag dumps used instructions.\n");
  printf("     l flag shows input language.\n\n");
  printf("     b - transpile into Brainfuck language.\n");
  printf("     o - transpile into Ook! language.\n");
  printf("     c - transpile into compressed Ook! language.\n");

  exit(EXIT_SUCCESS);
}

int main (int argc, char *argv[]) {
  char c;
  bool t_brainfuck, t_cook, t_ook;
  FILE *out;

  t_brainfuck = false;
  t_cook = false;
  t_ook = false;

  if (argc < 2) help(argv[0]);

  while ((c = getopt (argc, argv, "ilbco")) != -1) {
    switch (c) {
      case 'i':
        r_instr_dump = true;
        break;
      case 'l':
        r_lang_dump = true;
        break;
      case 'b':
        t_brainfuck = true;
        break;
      case 'c':
        t_cook = true;
        break;
      case 'o':
        t_ook = true;
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

  if (!(t_cook || t_ook || t_brainfuck )) {
    printf("No language was selected - transpiling into all supported languages\n");
    t_brainfuck = true;
    t_cook = true;
    t_ook = true;
  }

  if (t_brainfuck) {
    out = fopen("output.bf", "w");
    if (out != NULL) {
      printf("Generating Brainfuck code\n");
      generateBrainfuck(inst, out);
      fclose (out);
    } else
      printf("Cannot create output file for Brainfuck code\n");
  }
  if (t_cook) {
    out = fopen("output.cook", "w");
    if (out != NULL) {
      printf("Generating compressed Ook! code\n");
      generateCompressedOok(inst, out);
      fclose (out);
    } else
      printf("Cannot create output file for compressed Ook! code\n");
  }
  if (t_ook) {
    out = fopen("output.ook", "w");
    if (out != NULL) {
      printf("Generating Ook! code\n");
      generateOok(inst, out);
      fclose (out);
    } else
      printf("Cannot create output file for Ook! code\n");
  }

  free(inst);

  return EXIT_SUCCESS;
}
