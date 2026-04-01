# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Purpose

AI-assisted C study project implementing data structures from *Mastering Algorithms with C* (Kyle Loudon, O'Reilly), applying embedded TDD practices from *Test-Driven Development for Embedded C* (James W. Grenning). Each chapter is self-contained under its own directory (`Chapt_5/`, etc.) with its own makefile and test binary.

## Chapter Structure Pattern

Every chapter follows the same layout:

- Each data structure is its own **module** (`list/`, `dlist/`, `buffer_pool/`, etc.) with `inc/`, `src/`, and a `.mk` file
- Module `.mk` files register themselves by appending to `SRC_DIRS` and `INCLUDE_DIRS`; the chapter's top-level `makefile` includes them
- Tests live in `tests/` as `.cpp` files (CppUTest); C sources compile as C and are linked by `g++`
- Each chapter has its own `CLAUDE.md` with chapter-specific build commands and API docs

See [`Chapt_5/CLAUDE.md`](Chapt_5/CLAUDE.md) for the canonical example of chapter-level guidance.

## Adding a New Module to a Chapter

1. Create `module_name/{inc/module_name.h,src/module_name.c,module_name.mk}`
2. In `module_name.mk`, append to `SRC_DIRS` and `INCLUDE_DIRS`
3. Add `include module_name/module_name.mk` to the chapter's top-level `makefile`
4. Add `tests/ModuleNameTest.cpp` with a CppUTest group; `AllTests.cpp` picks it up automatically

## Coding Conventions

**Naming:**
- Struct tags: `struct Foo_`; typedef: `Foo` (PascalCase, no trailing underscore)
- Functions: `module_verb_noun` snake_case (e.g. `list_insert_next`)
- Constants/macros: `ALL_CAPS` (e.g. `BUFFER_POOL_CAPACITY`)

**Error returns:** `0` on success, `-1` on failure (NULL input, empty list, allocation failure).

**Generic data:** All container data is `void *` — no type-specific variants.

**Embedded-friendly patterns:**
- Pluggable allocator callbacks (`elem_alloc(ctx)` / `elem_free(ctx, elmt)`) allow heap-free operation
- Free-list pools reuse each node's own `next` pointer as the free-list link — declare pools as local or static variables, never on the heap
- Tunable capacities are `#define` constants in the module header

**Header guards:** Traditional `#ifndef MODULE_H / #define MODULE_H / #endif /* MODULE_H */`.

**Style:** Decorative `***` banner blocks separate functions in `.c` and `.h` files.

## Dependency

```sh
sudo apt install libcpputest-dev   # one-time
```
