# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Context

This is a C study project implementing data structures from *Mastering Algorithms with C* (Kyle Loudon, O'Reilly). Chapter 5 covers singly linked lists.

## Structure

```
list/
  inc/list.h   — struct definitions and macro-based accessors
  src/list.c   — function implementations
```

No build system or test harness exists yet. Compile manually with gcc:

```sh
gcc -I list/inc -c list/src/list.c -o list.o
```

## API Overview

**Structures:**
- `ListElmt` — node holding a `void *data` pointer and `*next` link
- `List` — container holding `size`, function pointers (`match`, `destroy`), and `head`/`tail` pointers

**Functions:**
- `list_init(list, destroy)` — initializes a list; `destroy` may be NULL
- `list_destroy(list)` — removes all elements, calling `destroy` on each data pointer
- `list_insert_next(list, element, data)` — inserts after `element`; pass NULL to insert at head
- `list_rem_next(list, element, data)` — removes element after `element`; pass NULL to remove head

**Macros:** `list_size`, `list_head`, `list_tail`, `list_is_head`, `list_is_tail`, `list_data`, `list_next`

## Known Bugs in Current Code

The files contain several bugs (likely transcription errors from the book):

- `list.h`: include guard uses `#ifdef` instead of `#ifndef`
- `list.h`: `match` parameter has typo `conts` instead of `const`
- `list.h` / `list.c`: `list_init` parameter typed as `Lust *` instead of `List *`
- `list.c`: `list_init` assigns to `list->detroy` (missing 's')
- `list.c`: `list_destroy` has misplaced parenthesis in `list_rem_next` call
- `list.c`: `list_insert_next` uses `listElmt` (wrong case) in `malloc` sizeof
- `list.c`: `list_insert_next` does `list->size--` instead of `list->size++`
- `list.c`: `list_remove_next` is misnamed — header declares it as `list_rem_next`
- `list.c`: `list_remove_next` parameter `data` typed as `const void *` but dereferenced as `void **`
