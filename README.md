# ElLerge-compiler

<p align="center">
  <strong>A small Pascal compiler built from scratch in C.</strong><br>
  Exploring the foundations of compilers through lexical analysis, parsing, semantic checks, and P-code generation.
</p>

<p align="center">
  <a href="https://github.com/AnEnthusiasticCoder/ElLerge-compiler">
    <img src="https://img.shields.io/badge/GitHub-ElLerge--compiler-181717?logo=github" alt="GitHub repository">
  </a>
  <img src="https://img.shields.io/badge/Language-C-blue?logo=c" alt="Language: C">
  <img src="https://img.shields.io/badge/Source%20Language-Pascal-7159c1" alt="Source language: Pascal">
  <img src="https://img.shields.io/badge/Project-Educational-orange" alt="Educational project">
</p>

## Overview

**ElLerge-compiler** is a compact compiler project written in C that processes a subset of Pascal and translates it into an intermediate **P-code** instruction format. It was created as a hands-on way to understand how compilers work internally, from recognizing source code tokens to generating instructions for a virtual stack-based machine.

Rather than relying on compiler-generation frameworks, the project implements the main stages directly: a lexical analyzer, a syntax analyzer, semantic checks, and an intermediate-code generator. A separate interpreter is included as an initial implementation of the P-code execution environment.

## Features

- **Lexical analysis** — recognizes Pascal keywords, identifiers, literals, operators, and punctuation.
- **Syntax analysis** — parses program structure, declarations, expressions, and supported statements.
- **Semantic checks** — includes checks for duplicate and undeclared identifiers, type compatibility, constant modification, and selected control-flow and array constraints.
- **P-code generation** — translates accepted constructs into stack-oriented intermediate instructions and writes them to `fichierSortie.po`.
- **P-code interpreter** — a separate C program with an initial implementation of the virtual machine's instruction execution.
- **Example programs** — includes sample inputs covering general language constructs, conditionals, arrays, and `repeat...until` loops.

## Language coverage

The front end is designed around a Pascal subset. The grammar and compiler code cover constructs such as:

| Area | Constructs |
|---|---|
| Declarations | `program`, `const`, `var`, and type declarations |
| Basic types | `integer`, `real`, `string`, `boolean`, and `char` |
| Expressions | Arithmetic operations, relational operators, and parentheses |
| Input/output | `read(...)` and `write(...)` |
| Conditions | `if...then...else` |
| Loops | `while...do`, `for...to`, `for...downto`, and `repeat...until` |
| Other constructs | Arrays, records, and `case` statements in the compiler's grammar/code-generation work |

> **Compatibility note:** ElLerge-compiler is a learning project, not a complete or standards-compliant Pascal implementation. Support varies by construct and by compilation stage. Some constructs are recognized or handled by the front end but are not yet fully supported by the interpreter.

## How it works

```text
Pascal source
     │
     ▼
Lexical analysis
     │
     ▼
Syntax analysis
     │
     ▼
Semantic checks
     │
     ▼
P-code generation
     │
     ▼
 fichierSortie.po
     │
     ▼
P-code interpreter (separate program)
```

The compiler uses a recursive-descent style parser and maintains symbol information while analyzing the source. Once the program is accepted, it emits P-code instructions that represent operations such as arithmetic, comparisons, memory access, input/output, and control flow.

## Example

A small Pascal program accepted by the project's sample grammar:

```pascal
program HelloElLerge;

var
  number: integer;

begin
  number := 10;

  if number > 5 then
    write(number)
  else
    write(0);
end.
```

## Project structure

```text
ElLerge-compiler/
├── Compiler.c           # Lexer, parser, semantic checks, and P-code generation
├── interpreteur.c       # Separate P-code interpreter
├── Grammaire.txt        # Language grammar, semantic rules, and P-code reference
├── fichierSortie.po     # Generated P-code output
└── tests/
    ├── testGeneral.txt
    ├── testIF.txt
    ├── test_array.txt
    └── test_until.txt
```

## Building

The project is written in C. With a MinGW-w64 GCC toolchain available, build the compiler from the repository root:

```bash
gcc Compiler.c -o ElLerge-compiler.exe
```

The interpreter is a separate executable:

```bash
gcc interpreteur.c -o ElLerge-interpreter.exe
```

The source currently uses case-insensitive string-comparison functions whose names can vary between C runtimes. Depending on your compiler and operating system, small compatibility adjustments may be needed.

## Running the compiler

The current `main()` function in `Compiler.c` uses a predefined input path:

```c
fichier = fopen("tests/testGeneral.txt", "r");
```

Run the compiler from the repository root so that this relative path resolves correctly:

```bash
.\ElLerge-compiler.exe
```

After compilation, the generated intermediate code is written to:

```text
fichierSortie.po
```

To compile a different sample, update the input path in `Compiler.c` and rebuild. Command-line input-file selection is a possible future improvement.

## Project status

<em>Work in progress — core compiler stages are implemented, while language coverage and runtime execution continue to evolve.</em>

- [x] Lexical analysis
- [x] Syntax analysis for the implemented Pascal subset
- [x] Initial semantic validation
- [x] P-code generation
- [x] Initial standalone P-code interpreter
- [ ] Complete interpreter support for every generated instruction
- [ ] Command-line source-file argument
- [ ] Broader automated test coverage and error reporting

## Goals

ElLerge-compiler is primarily a practical exploration of compiler construction. The long-term goal is to keep extending the language subset, improve diagnostics, strengthen semantic analysis, and make the generated P-code executable consistently through the interpreter.

## Repository

[AnEnthusiasticCoder/ElLerge-compiler](https://github.com/AnEnthusiasticCoder/ElLerge-compiler)

---

*Built as a learning project to explore the internals of compilers and programming languages.*
