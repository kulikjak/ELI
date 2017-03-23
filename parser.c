#include <stdio.h>
#include <stdlib.h>

#include "eli.h"
#include "input.h"
#include "parser.h"

// Enum for input symbol types.
typedef enum {
  LETTER, NUMBER, WHITE_SPACE, END, NO_TYPE
} InputCharType;

// Enum for all supported esoteric languages.
typedef enum {
  OOK, COMPRESSED_OOK, BRAINFUCK, UNKNOWN
} LanguageType;

// Parse function for each supported esoteric language.
Instruction* parse_ook(int size);
Instruction* parse_compressed_ook(int size);
Instruction* parse_brainfuck(int size);

LanguageType get_language(void);

static int character;  // input character
static InputCharType input;  // input symbol

// Sets input symbol type of loaded symbol.
void getInputType(void) {
  if ((character >= 'A' && character <= 'Z') ||
      (character >= 'a' && character <= 'z'))
    input = LETTER;
  else if (character >= '0' && character <= '9')
    input = NUMBER;
  else if (character == EOF)
    input = END;
  else if (character <= ' ')
    input = WHITE_SPACE;
  else
    input = NO_TYPE;
}

// Read one symbol from the input and get its symbol type.
void readInput(void) {
  character = getChar();
  getInputType();
}

// Try to match given character and current symbol.
// Error and file termination is caused if match is not successfull.
void match(char a) {
  if (a == EOF)
    error("Unexpected end of file.");
  if (a != character)
    error("Unexpected symbol.");
  readInput();
}

// Parse file and return array of instructions.
Instruction* parseFile(char* fileName) {
  int size;
  LanguageType lang;

  size = initInput(fileName);
  lang = get_language();

  if (r_lang_dump) {
    switch (lang) {
      case OOK:
        printf("== File is being interpreted as Ook! language ==\n");
        break;
      case COMPRESSED_OOK:
        printf("== File is being interpreted as compressed Ook! language ==\n");
        break;
      case BRAINFUCK:
        printf("== File is being interpreted as Brainfuck language ==\n");
        break;
      default:
        printf("Unknown language.");
    }
  }

  resetFilePtr();
  switch (lang) {
    case OOK:
      return parse_ook(size);
    case COMPRESSED_OOK:
      return parse_compressed_ook(size);
    case BRAINFUCK:
      return parse_brainfuck(size);
    default:
      error("Unknown language.");
  }
  return NULL;  // not reachable
}

// Get language based on input file.
// If language cannot be determined or is not supported,
// error and program termination occures.
LanguageType get_language(void) {
  readInput();
q0:
  switch (input) {
    case WHITE_SPACE:
      readInput();
      goto q0;
    case LETTER:
      if (character == 'O')
        return OOK;
    default:
      goto q1;
  }
q1:
  switch (input) {
    case WHITE_SPACE:
      readInput();
      goto q1;
    default:
      switch (character) {
        case '!':
        case '?':
          return COMPRESSED_OOK;
        case '<':
        case '>':
        case '+':
        case '-':
        case '[':
        case ']':
        case ',':
          return BRAINFUCK;
        case '.':
          readInput();
          goto q1;
        default:
          error("Cannot determine input language.");
      }
  }
  return UNKNOWN;
}

Instruction* parse_ook(int size) {
  int last = 0;
  Instruction* inst = (Instruction*) malloc (((int)(size/4) + 1) * sizeof(Instruction));

  readInput();
a0:
  switch (input) {
    case END:
      inst[last] = EXIT;
      return inst;
    case WHITE_SPACE:
      readInput();
      goto a0;
    case LETTER:
      match('O');
      match('o');
      match('k');
      switch (character) {
        case '.':
          readInput();
          goto a1;
        case '!':
          readInput();
          goto a2;
        case '?':
          readInput();
          goto a3;
      }
    default:
      error("Unexpected symbol.");
  }

a1:
  switch (input) {
    case END:
      error("Unexpected end of file.");
    case WHITE_SPACE:
      readInput();
      goto a1;
    case LETTER:
      match('O');
      match('o');
      match('k');
      switch (character) {
        case '.':
          inst[last++] = INC;
          readInput();
          goto a0;
        case '!':
          inst[last++] = SCANC;
          readInput();
          goto a0;
        case '?':
          inst[last++] = FWD;
          readInput();
          goto a0;
      }
    default:
      error("Unexpected symbol.");
  }

a2:
  switch (input) {
    case END:
      error("Unexpected end of file.");
    case WHITE_SPACE:
      readInput();
      goto a2;
    case LETTER:
      match('O');
      match('o');
      match('k');
      switch (character) {
        case '.':
          inst[last++] = PRINTC;
          readInput();
          goto a0;
        case '!':
          inst[last++] = DEC;
          readInput();
          goto a0;
        case '?':
          inst[last++] = BRZ;
          readInput();
          goto a0;
      }
    default:
      error("Unexpected symbol.");
  }

a3:
  switch (input) {
    case END:
      error("Unexpected end of file.");
    case WHITE_SPACE:
      readInput();
      goto a3;
    case LETTER:
      match('O');
      match('o');
      match('k');
      switch (character) {
        case '.':
          inst[last++] = BWD;
          readInput();
          goto a0;
        case '!':
          inst[last++] = BNZ;
          readInput();
          goto a0;
      }
    default:
      error("Unexpected symbol.");
  }
  return inst;  // not reachable
}

Instruction* parse_compressed_ook(int size) {
  int last = 0;
  Instruction* inst = (Instruction*) malloc ((size+1) * sizeof(Instruction));

  readInput();
b0:
  switch (input) {
    case END:
      inst[last] = EXIT;
      return inst;
    case WHITE_SPACE:
      readInput();
      goto b0;
    default:
      switch (character) {
        case '.':
          readInput();
          goto b1;
        case '!':
          readInput();
          goto b2;
        case '?':
          readInput();
          goto b3;
        default:
          error("Unexpected symbol.");
      }
  }

b1:
  switch (input) {
    case END:
      error("Unexpected end of file.");
    case WHITE_SPACE:
      readInput();
      goto b1;
    default:
      switch (character) {
        case '.':
          inst[last++] = INC;
          readInput();
          goto b0;
        case '!':
          inst[last++] = SCANC;
          readInput();
          goto b0;
        case '?':
          inst[last++] = FWD;
          readInput();
          goto b0;
        default:
          error("Unexpected symbol.");
      }
  }

b2:
  switch (input) {
    case END:
      error("Unexpected end of file.");
    case WHITE_SPACE:
      readInput();
      goto b2;
    default:
      switch (character) {
        case '.':
          inst[last++] = PRINTC;
          readInput();
          goto b0;
        case '!':
          inst[last++] = DEC;
          readInput();
          goto b0;
        case '?':
          inst[last++] = BRZ;
          readInput();
          goto b0;
        default:
          error("Unexpected symbol.");
      }
  }

b3:
  switch (input) {
    case END:
      error("Unexpected end of file.");
    case WHITE_SPACE:
      readInput();
      goto b3;
    default:
      switch (character) {
        case '.':
          inst[last++] = BWD;
          readInput();
          goto b0;
        case '!':
          inst[last++] = BNZ;
          readInput();
          goto b0;
        default:
          error("Unexpected symbol.");
      }
  }
  return inst;  // not reachable
}

Instruction* parse_brainfuck(int size) {
  int last = 0;
  Instruction* inst = (Instruction*) malloc ((size+1) * sizeof(Instruction));

  readInput();
c0:
  switch (input) {
    case END:
      inst[last] = EXIT;
      return inst;
    case WHITE_SPACE:
      readInput();
      goto c0;
    default:
      switch (character) {
        case '>':
          inst[last++] = FWD;
          readInput();
          goto c0;
        case '<':
          inst[last++] = BWD;
          readInput();
          goto c0;
        case '+':
          inst[last++] = INC;
          readInput();
          goto c0;
        case '-':
          inst[last++] = DEC;
          readInput();
          goto c0;
        case '.':
          inst[last++] = PRINTC;
          readInput();
          goto c0;
        case ',':
          inst[last++] = SCANC;
          readInput();
          goto c0;
        case '[':
          inst[last++] = BRZ;
          readInput();
          goto c0;
        case ']':
          inst[last++] = BNZ;
          readInput();
          goto c0;
        default:
          error("Unexpected symbol.");
      }
  }
  return inst;  // not reachable
}
