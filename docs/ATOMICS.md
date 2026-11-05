# Atomic memory operations (RV32A / RV64A)

Enable **A — Atomic memory operations** under **Build profile > Extensions**, or
start **New project from template > Atomic counter and reservation**. C/Assembly
builds use an `rv32ia`- or `rv64ia`-based architecture; ELF attributes preserve the selection.

## A word that is read and updated together

The nine word AMOs swap, add, XOR, AND, OR, or select a signed/unsigned minimum or
maximum. Each instruction returns the old word in `rd` and updates RAM in one
indivisible simulator event. `rd=x0` discards the result but still updates RAM.
Read and write watchpoints both see that event. Statistics count one read and one
write; the conceptual cache counts one transaction.

## A reservation that can expire

`lr.w rd,(rs1)` reads and reserves four bytes. `sc.w rd,rs2,(rs1)` writes only if
the reservation still matches. SC returns **0 on success, 1 on failure**; every
attempt consumes the reservation. It checks alignment and RAM bounds even when
there is no reservation. A failed reservation leaves RAM unchanged.

The most recent LR replaces an earlier reservation. An overlapping successful
RAM store, an overlapping debugger memory edit (even writing the same value),
any trap/interrupt entry, or MRET cancels it. A normal debugger pause preserves
it. Back/Replay, saved projects and comparison snapshots restore its exact state.

The instruction inspector displays the reservation and the last transaction.
`reserved()` returns 0 or 1. `reservation()` reads the address of an active
reservation; use `reserved() && reservation()==&counter` when it might be absent.
These expressions are read-only and require A to be enabled.

## Ordering and platform limits

The encoder exposes Relaxed, Release (`.rl`), Acquire (`.aq`) and both (`.aqrl`).
All four encodings are preserved. This platform interleaves one instruction at a
time from up to four harts sharing RAM. It does not enumerate weak-memory outcomes.
Naturally aligned four-byte accesses work in both XLENs; RV64 also supports all
eleven eight-byte `.d` operations. RV64 `.w` returns the old word sign-extended
to 64 bits and ignores the high half of its input value. LR/SC must match both
address and width. Writes by another hart invalidate overlapping reservations.
Atomics never call MMIO handlers. Zacas, byte/halfword atomics and standalone
Zaamo/Zalrsc profiles are not enabled.

`rv_atomic.h` supplies explicit LR/SC and AMO helpers. AMOs return the old word;
LR acquires, SC releases and the AMO helpers use both ordering bits. Compiler
`__atomic` builtins are the alternative for ordinary portable C source. The SDK
helpers do not create threads or synchronization with the host. On RV64, the
`rv_atomic_*64` helpers provide the eight-byte forms; the original helpers remain
32-bit on either architecture.

The RV32 reconstructed C route preserves atomic operations as explicit machine helpers. These
need shared simulated RAM, precise faults and LR/SC state; they are not plain C
arithmetic. Stores within an atomic function also use a helper so they invalidate
overlapping reservations. Separate function exports still require a caller-supplied
machine model at call boundaries.

The semantics follow the [RISC-V A specification](https://docs.riscv.org/reference/isa/unpriv/a-st-ext.html).

The RV64 profile supports word (`.w`) and doubleword (`.d`) atomic operations.
