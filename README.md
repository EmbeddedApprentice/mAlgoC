# Mastering Algorithms with C — AI-Assisted Study Project

A project series combining AI-assisted code generation with practical embedded systems development.
Each chapter implements a data structure from *Mastering Algorithms with C* (Kyle Loudon, O'Reilly)
with professional embedded development practices applied throughout.

> Originally described in the blog post
> [A More Embedded AI Application](https://embeddedapprentice.com/a-more-embedded-ai-application/)
> — *EmbeddedApprentice.com, February 25, 2026*

---

## Motivation

AI + microcontroller searches mostly surface hardware accelerator marketing — practical guidance on
using AI tools in day-to-day embedded C development is scarce. This project uses a well-understood
algorithms textbook as a controlled environment to learn AI-assisted code generation before applying
the same workflow to larger embedded initiatives.

The approach mashes together two established references:
- *Mastering Algorithms with C* — Kyle Loudon (data structures and algorithms)
- *Test-Driven Development for Embedded C* — James W. Grenning (TDD workflow and tooling)

---

## Professional Practices Applied Per Chapter

Each chapter targets the following across every data structure implementation:

| Practice | Tool / Approach |
|---|---|
| Build system | Makefile |
| Unit tests | CppUTest |
| Documentation | Doxygen |
| Multi-compiler | gcc / clang |
| Lint | cppcheck / clang-tidy |
| Formatting | clang-format |
| Memory checking | Valgrind |
| Logging / tracing | configurable trace macros |

---

## Chapters

### Chapter 5 — Singly Linked List [`Chapt_5/`](Chapt_5/)

Implements `List` and `ListElmt` from Chapter 5 of Loudon.
Fixes all transcription bugs present in the book's printed source,
and provides a full CppUTest harness with unit and stress tests.

**Build and run tests:**
```sh
cd Chapt_5
sudo apt install libcpputest-dev   # first time only
make
```

---

## Dependencies

- `gcc` or `clang`
- `libcpputest-dev` (Ubuntu/Debian: `sudo apt install libcpputest-dev`)
