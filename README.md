Minimal Brainfuck interpreter
=============================

A minimal Brainfuck interpreter implemented in ANSI C that evaluates
Brainfuck source code and writes output to a provided `FILE` stream.

## Description

The function `mbfi_eval` interprets Brainfuck code encoded as a sequence
of token characters (macros from `mbfi.h`) and operates on a contiguous
tape buffer allocated.  The function advances through code, updates the
tape, handles loops, performs I/O, and writes printed characters to the
supplied `FILE *output`.

Behavior summary:

  - `BF_ADD / BF_SUB`: increment/decrement current cell
  - `BF_ARI / BF_ALE`: move tape pointer right/left
  - `BF_BLE / BF_BRI`: loop start/end handling (skips or rewinds to
    matching bracket)
  - `BF_PUTCH`: write current cell as char to output and flush
  - `BF_GETCH`: read a char from stdin into current cell
  - Non-token characters are ignored
  - Allocated tape is freed before returning

## API usage example

    #include <stdio.h>
    #include <mbfi.h>

    int main(void) {
        char *program = "++>+<[->+<].";
        mbfi_eval(program, stdout);
        return 0;
    }

## CLI usage examples

Run the program reading input from specified file:

    $ mbfi ~/dev/src/bf/program.bf

Run the program reading input piped from `stdin`:

    $ cat ~/dev/src/bf/program.bf | mbfi -

## License

  - MIT License
  - Copyright (c) 2018-2025, J. A. Corbal
  - Read the file [`LICENSE`](LICENSE) for more details
