Minimal Brainfuck interpreter
=============================

A minimal Brainfuck interpreter function implemented in C that evaluates Brainfuck source code and writes output to a provided `FILE` stream.

## Description

`evaluate_bf` interprets Brainfuck code encoded as a sequence of token
characters (macros from `mbfi.h`) and operates on a contiguous tape
buffer allocated with `calloc(MAX_BUFFER, sizeof(char))`.  The function
advances through code, updates the tape, handles loops, performs I/O,
and writes printed characters to the supplied `FILE *output`.

Behavior summary:

  - `BF_ADD / BF_SUB`: increment/decrement current cell
  - `BF_ARI / BF_ALE`: move tape pointer right/left
  - `BF_BLE / BF_BRI`: loop start/end handling (skips or rewinds to
    matching bracket)
  - `BF_PRINT_CH`: write current cell as char to output and flush
  - `BF_GET_CH`: read a char from stdin into current cell
  - Non-token characters are ignored
  - Allocated tape is freed before returning

## Usage example

    #include <stdio.h>
    #include <mbfi.h>

    int main(void) {
        char *program = "++>+<[->+<].";
        evaluate_bf(program, stdout);
        return 0;
    }

## License

  - MIT License
  - Copyright (c) 2025, J. A. Corbal
  - Read the file [`LICENSE`](LICENSE) for more details
