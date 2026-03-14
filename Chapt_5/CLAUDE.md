# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Context

C study project implementing data structures from *Mastering Algorithms with C* (Kyle Loudon, O'Reilly). Chapter 5 covers linked lists: singly-linked (`list/`) and doubly-linked (`dlist/`).

## Build & Test

Uses **CppUTest** (detected via `pkg-config`) and a recursive-include makefile pattern.

```sh
make              # build and run all tests with -v
make tests        # same as all
make test_link    # run only singly-linked list tests (group SList)
make test_dlink   # run only doubly-linked list tests (group DList)
make test_bufpool # run only buffer pool tests (group BPool)
make clean        # remove objs/ and test binary
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
- `List` — container: `size`, `match`, `destroy`, `head`/`tail`, plus pluggable `elem_alloc`/`elem_free` callbacks and `allocator_ctx`
- `list_init(list, destroy)` — `destroy` may be NULL; defaults to system `malloc`/`free` for nodes
- `list_init_with_allocator(list, destroy, ctx, alloc_fn, free_fn)` — inject a custom node allocator; `alloc_fn(ctx)` returns a `ListElmt`-sized chunk or NULL, `free_fn(ctx, elmt)` returns it
- `list_destroy(list)` — drains list, calls `destroy` on each data ptr
- `list_insert_next(list, element, data)` — NULL element → insert at head
- `list_rem_next(list, element, data)` — NULL element → remove head; returns -1 on error
- Macros: `list_size`, `list_head`, `list_tail`, `list_is_head`, `list_is_tail`, `list_data`, `list_next`

### ListElmtPool — zero-heap node allocator

Reuses each `ListElmt`'s own `next` pointer as the free-list link (same embedded-free-list trick as `BufferPool`). Declare one as a local or static variable — no `malloc` needed.

- `LIST_ELMT_POOL_CAPACITY` (32) — compile-time pool size
- `list_elmt_pool_init(pool)` — links all nodes into the free list
- `list_elmt_pool_alloc(ctx)` / `list_elmt_pool_free(ctx, elmt)` — match the `elem_alloc`/`elem_free` callback signatures; pass directly to `list_init_with_allocator`

## API — Doubly-Linked List (`dlist/`)

Mirrors the singly-linked API with `dlist_` prefix. `dListElmt` adds a `*prev` pointer.

- `dlist_init`, `dlist_destroy`, `dlist_insert_next`, `dlist_rem_next` — same semantics as list counterparts
- Macros: `dlist_size`, `dlist_head`, `dlist_tail`, `dlist_is_head`, `dlist_is_tail`, `dlist_data`, `dlist_next`, `dlist_prev`

Tests in `tests/DListTest.cpp` (group `DList`) include `prev`-pointer correctness tests.

## Module — Buffer Pool (`buffer_pool/`)

Zero-heap, statically-allocated buffer pool. All memory lives in a `BufferPool` struct (declare as a local or static variable — no `malloc` required). Each `Buffer` embeds its own `next` pointer so the free list needs no separate heap nodes.

- `BUFFER_DATA_SIZE` (256) and `BUFFER_POOL_CAPACITY` (16) are compile-time constants in `buffer_pool.h`
- `buffer_pool_init(pool)` — links all buffers into the free list; call once at startup
- `buffer_pool_alloc(pool)` — pops a buffer; returns NULL when exhausted
- `buffer_pool_free(pool, buf)` — pushes a buffer back; LIFO order
- `buffer_pool_free_count(pool)` / `buffer_pool_min_free(pool)` — instrumentation

Zero-heap guarantee: `AllocResultIsWithinStorageArray` test confirms every returned pointer falls within `pool.storage[]`.
