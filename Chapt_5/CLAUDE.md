# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Context

C study project implementing data structures from *Mastering Algorithms with C* (Kyle Loudon, O'Reilly). Chapter 5 covers linked lists: singly-linked (`list/`) and doubly-linked (`dlist/`, in progress).

## Build & Test

Uses **CppUTest** (detected via `pkg-config`) and a recursive-include makefile pattern.

```sh
make          # build and run all tests with -v
make clean    # remove objs/ and test binary
```

Tests live in `tests/` as `.cpp` files; C sources compile as C, linked by `g++`.

To run a single test group, the test binary accepts CppUTest arguments:
```sh
./Chapt_5_tests -g SList -v
```

## Module System

Each module registers itself by appending to `SRC_DIRS` and `INCLUDE_DIRS` in its own `.mk` file, which the top-level `makefile` includes:

```
list/list.mk   → SRC_DIRS += list/src  |  INCLUDE_DIRS += list/inc
dlist/dlist.mk → SRC_DIRS += dlist/src |  INCLUDE_DIRS += dlist/inc
```

`INC_FLAGS` is derived automatically from `SRC_DIRS` via the `%/src=%/inc` substitution, so the `INCLUDE_DIRS` entries in module `.mk` files are redundant but harmless. To add a new module, create a `.mk` file and `include` it in `makefile`.

## API — Singly-Linked List (`list/`)

- `ListElmt` — node: `void *data`, `*next`
- `List` — container: `size`, `match`, `destroy`, `head`/`tail`
- `list_init(list, destroy)` — `destroy` may be NULL
- `list_destroy(list)` — drains list, calls `destroy` on each data ptr
- `list_insert_next(list, element, data)` — NULL element → insert at head
- `list_rem_next(list, element, data)` — NULL element → remove head; returns -1 on error
- Macros: `list_size`, `list_head`, `list_tail`, `list_is_head`, `list_is_tail`, `list_data`, `list_next`

## API — Doubly-Linked List (`dlist/`)

Mirrors the singly-linked API with `dlist_` prefix. `dListElmt` adds a `*prev` pointer.

- `dlist_init`, `dlist_destroy`, `dlist_insert_next`, `dlist_rem_next` — same semantics as list counterparts
- Macros: `dlist_size`, `dlist_head`, `dlist_tail`, `dlist_is_head`, `dlist_is_tail`, `dlist_data`, `dlist_next`, `dlist_prev`

Tests in `tests/DListTest.cpp` (group `DList`) include `prev`-pointer correctness tests.
