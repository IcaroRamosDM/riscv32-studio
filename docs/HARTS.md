# Multiple hardware threads

The sections below describe the classic workspace. The advanced workspace now
has separate per-hart completion, deterministic scheduling, ELF TLS and a private
locked libc, hosted C17 threads and cooperative waits; see [Advanced platform](ADVANCED_PLATFORM.md)
and [C17 runtime](ADVANCED_C17_RUNTIME.md). The classic route described below
retains its separate hardware-hart interface and does not provide threads.h.

Teaching and M-only firmware machines can run **one to four harts**. Each hart has its own PC,
integer/floating/vector registers, CSRs, LR/SC reservation, call depth and exit
state. RAM, console, GPIO, display and UART are shared. The supported RV32
profiles (including Zve32x) and RV64I/M profiles retain their existing limits.
Machine firmware has a shared clock and independent per-hart deadlines and IRQ
levels. See [Firmware](FIRMWARE.md) for the virtual platform map and controls.

## Using the desktop

Choose **Harts = 2–4** before building C, or open the **Shared atomic counter**
template. Stack size is per hart: four harts with 4 KiB stacks reserve 16 KiB.
The Harts tab selects the CPU being inspected and stepped. Its expanded view
shows all PCs, stack pointers, retired counts and exit states together.

Run uses the selected scheduling mode:

- **Round robin:** one instruction attempt per runnable hart, in increasing ID
  order with wraparound. Exited harts are skipped.
- **Selected hart only:** run the inspected hart while the others remain still.

Instruction and C Into/Over/Out always use the selected hart. Selection itself
does not execute, edit or discard history. Back/Replay follows the global event
order and selects the hart that produced the event. It restores shared writes,
device state and all affected reservations, including partial vector stores.
Breakpoints are shared; use `hartid == 1` as a condition for one hart. Expressions
`hartid` and `harts` expose the inspected ID and total count.

Every hart's `main` return, Teaching exit ECALL, or firmware MMIO exit ends only that hart. The machine exits
when all harts have exited. Its exit code is the first nonzero per-hart code in
ID order, or zero. A trap pauses execution for inspection. Editing a hart's PC
can revive it; Undo restores its prior exit state.

## C, Assembly and ELF

```sh
python3 tools/build_guest.py --source demos/c/hart_counter.c \
  --arch rv32ima_zicsr --harts 4 --stack 4KiB --output build/guest/harts
./build/debug/riscv32-studio --elf build/guest/harts.elf --run --json --observe result
```

The example produces `result = 400`. `runtime/include/rv_hart.h` provides
`rv_hart_id()` and `rv_hart_count()` through Teaching ECALLs 0x140 and 0x141,
or through mhartid/MMIO in firmware.
With Zicsr, `mhartid` also returns the hart's ID. Startup assigns separate stacks,
lets hart 0 clear BSS, and releases the others through a shared initialization
barrier. It passes ID/count in a0/a1 to main; ordinary `main(void)` is supported.
TLS, an operating system, pthreads and a dynamic hart-start service are absent.

ELF32/64 uses a nonallocated `.riscv.studio` section containing four little-endian
32-bit words: magic `0x54485352`, version 1, count 1–4 and stack stride. Stride is
a multiple of 16; zero is allowed for Assembly-managed stacks. The loader checks
the section, rejects duplicates and incompatible counts, and infers the hart
count automatically. Assembly export preserves this metadata. Raw formats lose
metadata: pass `--harts N` explicitly and initialize the stacks yourself unless
the image already includes the compiled runtime. Images without metadata keep
the single-hart default.

The terminal commands `hart [N]` and `schedule [round-robin|selected]` list/select
harts and configure Run. Multi-hart batch JSON version 3 contains every hart's
registers, PC, CSRs, floating/vector state, reservation and exit code; its global
retired count is the sum. Per-hart 64-bit values use decimal strings.

## Debugging, persistence and memory ordering

Projects and comparisons preserve all hart banks, scheduler position, selected
hart, partial exits and retained past/future history. Comparison reports include
every bank. Multi-hart files use project version 4 so older applications reject
them instead of silently losing CPU banks; existing single-hart versions remain
readable. The educational pipeline/cache estimate analyzes the selected hart's
retained instructions only; it excludes other harts, coherence, contention and
scheduling latency. It is not a multicore timing measurement.

GDB exposes hart N as thread N+1. Thread enumeration/selection, register edits,
single stepping, shared breakpoints/watchpoints and global reverse history are
supported. Both `Hc` and bounded `vCont;c/s` requests are handled; signal delivery
and non-stop/multiprocess modes are not advertised. A resumed GDB request restores
the desktop's scheduling preference when it stops. `reverse-stepi` undoes the
latest global instruction, which can belong to another hart after a shared run.
Protocol semantics follow the [GDB remote packet documentation](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Packets.html).

The simulator uses a deterministic, sequentially consistent shared-memory model.
An atomic instruction executes indivisibly between scheduler turns. A write by
another hart to a reserved four/eight-byte value invalidates LR/SC even if its value is
unchanged; history restores the exact reservation on undo. This behavior follows
the relevant [RISC-V atomic reservation requirements](https://docs.riscv.org/reference/isa/unpriv/a-st-ext.html).
It does not explore RVWMO reorderings, cache coherence protocols, all possible
interleavings, hardware races or parallel host execution. Use manual selection
to explore a particular ordering. Runnable C export supports all configured harts through its public runtime,
including shared reservations, complete vector banks and per-hart firmware IRQs.
It starts from the initial image in round-robin mode and preserves bounded
continuations. The runner supports `--hart N --schedule selected|round-robin`;
its JSON includes every bank and the next scheduling turn. Exporting preserves
the paused desktop machine. Direct native function calls remain limited to
single-hart Teaching programs; whole-program library execution supports all harts.


## Explore schedules

The Harts inspector provides **Explore schedules...** while a machine with two
to four harts is paused and unfinished. It captures the current machine and,
from the main workspace, its sources/build settings. Experiments operate on
independent copies. Existing breakpoints do not limit those copies; the selected
attempt limit, a trap, completion or Cancel stops an experiment.

Choose 2–16 experiments, 1–1,000,000 instruction attempts per experiment, a
32-bit seed and a maximum burst of 1–64 instructions. The first two schedules
are forward/reverse round robin, one attempt per hart. The remaining schedules
use xorshift32 to select a runnable hart and a bounded burst. This explores a
finite set of sequential instruction orders; it is not a weak-memory model or
proof of race freedom. A limit can indicate starvation or slow progress, not
necessarily a deadlock.

Integer observations such as `u32(&counter)` make intended results comparable.
Each report records the final selected hart for register-dependent expressions,
per-hart exit codes, exact output bytes, RAM hashes and the complete sequence of
executed hart IDs. Stack temporaries can change RAM hashes without changing the
intended result. Missing observations and limit/cancelled runs are inconclusive.

Save a report to keep the frozen starting project, algorithm version, Studio
version, seed, limits and results. Loading that report restores its starting
snapshot and settings; Run regenerates the experiments. Reproduction assumes
the same simulator semantics. Save a selected result as a regular project to
inspect its sources, registers and up to 1,024 retained history entries. The
original machine is unaffected. The report is capped on input at 160 MiB, its
embedded project uses the normal 128 MiB limit and normal project validation.
