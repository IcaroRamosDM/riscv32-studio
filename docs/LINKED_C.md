# Runnable C export

Pause a program and open **Reconstructed C → Export → Export runnable C project...**.
Choose a new folder. Files are staged before installing the folder; an existing
destination is never replaced. Current machine state, history and source drafts
remain available in Studio.

For a small window, **View → Compact workspace tabs** (`Ctrl+Alt+T`) gives the
source its own workspace tab. In the reconstructed view, the function selector
replaces the redundant source-file header; the Export menu groups all C formats.

The project contains connected C functions, a checked runtime, the complete
initial RAM snapshot, a runner and a hash/mapping manifest. Build and run with:

```sh
make
./reconstructed image.bin --steps 1000000
./reconstructed image.bin --input input.bin --ram-out final-ram.bin
```

`--arg0 N` sets initial guest a0. The runner prints JSON with status, PC, all
integer, floating and vector registers, FCSR, vector controls, the atomic reservation, machine CSRs,
counts, guest exit value, console bytes, GPIO, full UART/frame/FIFO state,
display configuration and firmware timers/interrupts. The 64-bit CSR
counters are decimal strings, preserving values above JSON's exact numeric range.
For RV64, registers, PC, addresses, CSRs and instruction counts are also decimal
strings. The guest exit status remains a 32-bit value.
Its process exit is zero when the guest reaches its exit service; inspect the
separate guest exit value. Other execution stops return one; invalid files or
arguments return two.

Functions operate on explicit guest registers of the selected XLEN and RAM offsets. A bounded
dispatcher preserves calls, returns, tail transfers, function pointers and guest
recursion. Guest recursion uses simulated RAM instead of the host C stack.
Every instruction checks its budget and original bytes. `image.bin` includes
initial RAM outside the executable span. This is the initial program, not the
currently paused CPU or edited current memory.

The subset covers RV32I/M/A/F/D/C and RV64I/M/A/F/D/C, Zicsr, Zifencei and Zba/Zbb/Zbc/Zbs. Loads/stores
retain bounds and alignment checks. Atomics preserve four- and eight-byte LR/SC
reservations, including invalidation and precise faults. CSR instructions preserve
read-before-write, counter-write suppression and the previous inhibit mask.
Binary32 and binary64 use the bundled, pinned SoftFloat 3e sources with five rounding modes,
NaN bits, accumulated flags and FS/SD state; it never depends on host floating
point. RV64 includes full-width integer conversions and sign extension of W/WU results.
Compressed forms follow the selected XLEN, including C.LD/C.SD in RV64.

The Teaching convention includes exit, console bytes, LEDs, buttons,
retired-instruction ticks, UART and RAM-backed text/bitmap displays. Other device
ECALLs stop explicitly as unsupported services. Machine firmware includes
MRET/WFI, precise traps, direct/vectored software/timer/external interrupts and
the Studio MMIO map. The timer advances per attempt; UART advances per retired
instruction. Firmware requires Zicsr. Unsupported CSR addresses and illegal
dynamic rounding modes retain architectural traps.

Static analysis discovers a bounded set of instruction addresses. Export also
follows possible ECALL continuations and locally resolved JALR targets. For
firmware, it covers all declared code ranges within existing analysis bounds,
including anonymous trap handlers and exception continuations. This is coverage,
not a claim that every decoded byte is reachable. Illegal words in firmware code
produce guest traps if reached; Teaching rejects unsupported instructions before
creating a destination. Runtime
transfers use actual register/RAM values. Aligned RAM destinations outside the discovered
set stop as `unknown-target`; invalid full-width addresses retain precise faults; modified instruction bytes stop as `changed-code`.
New and self-modifying code are not executed. Function boundaries are estimates;
the manifest records them, instruction-to-C line mappings and file SHA-256 hashes.

Readable inferred C remains a separate export. Runnable C preserves explicit
machine state; it does not recover original source, types, signatures or a native
C calling convention. It does not claim physical timing or hardware emulation.
`program.h` exposes initialization and bounded continuation after a `step-limit`
stop. `make library host-call` also builds a native C17 library and a command-line
function-call example. This optional interface passes explicit ILP32 or LP64 words in
a0-a7, returns a0/a1, supplies bounded RAM copies and validates the chosen entry
and stack. It supports Teaching programs, whose known guest ABI remains the
caller's responsibility. There is no automatic native-pointer, structure,
floating-argument or variadic conversion. Read the exported `NATIVE_API.md` for
initialization, gp/sp/tp, recursion and continuation rules. Normal returned calls
can be repeated while retaining globals. Other stopped states require reset.

Keep the runtime and third-party license notices with your exported project.

Manifest version 5 records `xlen` and an explicit native ABI. The original
`StudioCall` interface rejects RV64 states; RV64 uses `StudioCall64` and the
`studio_call_begin64`/`studio_call_result64` functions. High guest addresses never
alias their low 32-bit values. Code generation shares only portable arithmetic
helpers with readable reconstruction; exported projects contain no private CPU,
instruction decoder or simulator core.

Zve32x/Zve32f export supports VLEN32/64/128 with ELEN32 on both XLENs. The
manifest records `vectorLength` and `vectorElementBits` (zero for scalar profiles).
Generated functions pass validated operands to the public vector modules; they
do not distribute a private instruction decoder. Memory groups, masks, tails,
fixed-point rounding/saturation, FP flags/FS/VS, vstart and partial/first-fault
loads preserve the simulator contract. A vector instruction consumes one
attempt, regardless of its lane count. Interrupted/failed instructions retain
the completed lanes and precise trap state. Full V, FP16/FP64 and other VLENs
are not enabled by this export.

The `linked_vector` tests compile the two C/Assembly examples with GCC/Clang
for every XLEN/VLEN, in Teaching and firmware, and compare all machine state
at intermediate budgets and completion. Separate cases cover memory faults,
vector CSRs, traps that resume a partial load, VS-off recovery and interrupts.
The inferred C view uses explicit `rv32_vector` / `rv64_vector` helpers because
its optimized scalar variables cannot replace a shared vector/FP/RAM context.

Multi-hart C export remains separate work.

Manifest version 7 adds `flen`. Floating banks in `StudioState` and `StudioHart`
use `uint64_t` slots for both FLENs; F-only slots retain their raw 32-bit values.
D exports include the binary64 SoftFloat subset and `studio_double.c`; F-only
exports retain their smaller source subset. FLEN64 reports use decimal strings.
Recompile native callers with the generated headers and library: the state layout
has changed, so old compiled objects must not be mixed with this runtime.
The generated Makefile tracks compiler/flags to rebuild objects when they change.
