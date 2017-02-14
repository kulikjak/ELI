# ELI
ELI (Esoteric Language Interpreter) is interpreter and transpiler of several simple esoteric languages written in C++. Right now it supports Brainf*ck, Ook! and compressed Ook! language.
* GitHub repo: https://github.com/Kulikjak/ELI

### Installation
Download this repository and run `make` to compile Interpreter and Transpiler. 
If you want to use only one part of the program, you can also run `make Interpreter` to make only the interpreter part, `make Transpiler` to make only the transpiler.

Program does not use any other libraries and so it should be compiled without any problem on any Unix like OS.

### Usage
*Interpreter* can automatically detect given language (and it's not dependent on the file suffix).
*Transpiler* can also detect input language and without any aditional flags it will transpile your input to all supoorted languages. You can specify just one with flags.

For more information run the program with `-h` flag (or simply incorrecly ;))

### Example
Interpreter:

	?> ./Interpreter source.bf
	Hello, I am the interpreter.

Transpiler:

	?> ./Transpiler source.bf
	No language was selected - transpiling into all supported languages
	Generating Brainfuck code
	Generating compressed Ook! code
	Generating Ook! code

### Author
* Jakub Kulik, <kulikjak@gmail.com>

