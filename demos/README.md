# Examples

Open these programs through the Examples menu or File > Open C / Assembly source.
Build, Load build and Step instruction show how each program executes.

| C example | Focus |
| --- | --- |
| c/learning.c | Functions, arrays, local storage and a complete first run. |
| c/decisions.c | Signed/unsigned comparisons and conditional branches. |
| c/loops.c | Loop counters, arithmetic and backward branches. |
| c/functions.c | Calls, arguments, return values and recursion. |
| c/arrays.c | Adjacent elements and indexed memory access. |
| c/pointers.c | Reading the same storage through pointers and byte accesses. |
| c/stack.c | Local storage and stack frames. |
| c/arithmetic.c | Arithmetic operations and compiler support routines. |
| c/optimization.c | Comparing learning (-O0) and optimized (-O2) builds. |
| c/stress.c | Larger RAM use, bounded execution batches and retained history. |

ram_boundary.asm performs an access at 0x00010000: it fails with 64 KiB of RAM
and succeeds with 128 KiB. Inspect the bytes at that address after the successful
run. Standalone Assembly supplies its own entry point and any required startup.

The .words files are small encoded programs that can be opened through
File > Load ELF / binary / words. They are not Assembly source files.

[The complete user manual](../HOW_TO_USE.md) contains full source listings,
exact procedures, expected outcomes and explanations for the guided lessons.
[How RISC-V works](../HOW_RISCV_WORKS.md) explains instruction encoding,
registers, the stack and memory addressing.
