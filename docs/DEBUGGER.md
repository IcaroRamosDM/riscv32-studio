# Debugging and Execution History

The desktop and monitor use the same C debugger and DWARF inspector. Assembly
instructions remain the execution unit. Source controls execute instructions
until their stopping condition is reached; they never execute C directly.

## Bit fields and optimized scalar locations

Typed inspection supports signed and unsigned integer bit fields of 1–64 bits,
including packed fields crossing byte boundaries, nested fields and arrays.
It reads `DW_AT_data_bit_offset` and legacy storage-unit layouts, including the
negative offsets emitted by GCC for packed fields. Every access is bounded by
the described containing object and RAM. Bit fields have a displayed storage
location but no addressable C object: `row.bits + 1` works; `&row.bits` is rejected.

Complete scalar locations made from `DW_OP_piece` or `DW_OP_bit_piece` can combine
registers, RAM and computed values, up to 64 bits in total. The reader verifies
the declared type size and each register's available width. Missing pieces,
unavailable registers, invalid ranges, oversized values and unassembled tails
remain unavailable. Aggregates outside contiguous RAM and entry-value expressions
are still outside this bounded support. Watch arithmetic remains XLEN-wide.

The implementation follows the bit/location conventions of the
[DWARF standard](https://dwarfstd.org/doc/DWARF5.pdf) for the supported little-endian
RISC-V targets.

## First desktop walkthrough

1. Open **Examples > learning.c**, or start with the default editor.
2. Select **Learning (-O0)** and click **Build & load**. Wait for **Ready**.
   The **Loaded** line identifies the actual program configuration.
3. Click the line-number gutter at line **22** (the assignment to result).
   A red **B** marks a source breakpoint. **Breakpoints** lists its enabled state.
4. Click **Run** (F5). Execution stops before entering that mapped C line.
5. Clear that breakpoint's checkbox, then click **Into C** (F11).
   This enters sum. Click Into C again to pass the parameter-saving prologue.
6. Open **Variables / stack**. Frame **#0 sum** shows count=5 and items pointing
   to an array beginning with 1. Select **#1 main** to see local=[10,20,30].
7. Double-click items in frame #0 to navigate RAM to its target.
8. Click **Out** (Shift+F11), then **Over** (F10). Execution reaches line 23
   after both sum calls; result is 72 and calls is 2.
9. Run to exit. Click **Back** (Alt+Left) to undo the exit attempt.
   Click **Replay** (Alt+Right) to restore the recorded exit.
10. Open **History** for instruction words, effects and the applied/future boundary.

The editor stays visible while startup executes. Runtime source remains available
in **Loaded source (read-only)**; startup does not replace the C editor automatically.
Opening a C file loads text into the editor; use Build & load to execute it.
Each **?** opens a short explanation, with **More in Help** for details.

## Controls and stopping rules

| Desktop | Monitor | Meaning |
| --- | --- | --- |
| Step instruction / F6 | step [N] | Execute actual instructions, ignoring breakpoints for these explicit steps. |
| Run / F5 | run [LIMIT] | Execute until breakpoint, exit, trap, cancellation, or attempt limit. |
| Into C / F11 | cstep [LIMIT] | Stop at the next different mapped C line; enter calls. |
| Over / F10 | next [LIMIT] | Advance a C line, running calls until the starting call depth is restored. |
| Out / Shift+F11 | finish [LIMIT] | Stop when an observed call returns to its caller. |
| Pause / Shift+F5 | Ctrl-C / stop | Pause between instructions. |
| Back / Alt+Left | back [N] | Restore the state before recorded instructions. |
| Replay / Alt+Right | forward [N] | Restore recorded future states without executing instructions. |
| Reset / Ctrl+R | reset | Restore the original machine and clear history; retain breakpoints. |

C controls require line metadata. Starting in startup/unmapped code, Into C and
Over seek the first mapped C line. A backward jump to the same source location
also ends a source step, so a one-line loop can be inspected repeatedly.
All controls obey enabled breakpoints, except explicit instruction Step.
The instruction limit also applies to source/function controls and prevents an
endless loop from blocking the UI or monitor.

Over/Out track observed ABI calls through JAL/JALR link registers x1/x5 and
standard returns through those registers. Recursion is supported. An Out request
without an observed caller is rejected. Nonstandard Assembly conventions,
nonlocal jumps and manually changed registers can invalidate this depth model.
Tail jumps do not increase call depth. Inlined functions have no separate machine
call; source order and function boundaries can differ after optimization.
An Out stop occurs immediately after the return, even if the caller has no C map.

## Breakpoints

- Up to 128 address/source entries per loaded session.
- Double-click an Assembly instruction, or select it and press F9, to toggle an
  address breakpoint. The PC column displays B and the next-instruction marker.
- Click the source gutter, or focus a C line and press F9, for a mapped source
  breakpoint. Lines without instructions report that no breakpoint can be set.
- Breakpoints stop **before** the selected instruction executes.
- A source breakpoint stops on entry to its mapped line, including backward
  revisits. It does not stop on every consecutive instruction from that line.
- Run from a breakpoint passes its current occurrence once; a later loop
  iteration or recursive call can hit it again.
- Disable, enable or remove entries in the Breakpoints tab.
- Reset retains entries. Loading or building a replacement program clears them.

Monitor commands:

    break sum
    break 0x1044
    breakline 22 FILE_INDEX
    breaks
    disable 1
    enable 1
    delete 1

Breakpoint IDs start at 1. Use sources to find FILE_INDEX (zero-based).
Without an explicit file index, breakline uses the current mapped file, or index
zero when there is no current mapping. During startup that may be an Assembly
file; use cstep first or provide the index.

## Variables, call frames and pointers

The inspector reads DWARF debug metadata, current registers, and RAM. It presents
parameters, local variables in visible scopes, and globals from the active
compilation unit. The nearest scope wins when names are shadowed.

    bt
    frame 1
    locals
    print local
    frame 0

Frame indices start at zero. The selected frame changes inspection, not execution.
Execution/history controls return selection to frame zero. Caller PCs point to
the calling instruction; the top frame PC is the next instruction.

Call-frame information (CFI) reconstructs saved registers and the caller's stack
pointer. Older frames stop when metadata is missing, a required value cannot be
recovered, or a repeated frame is detected. Missing CFI is reported instead of
guessing older stack frames from arbitrary RAM. The inspector displays up to 64
frames and 256 variables, with limit indicators.

Supported presentations:

- Integer scalars from 1 through 8 bytes, with signed decimal and hexadecimal.
- Register, computed, constant, and RAM locations when the expression is supported.
- Small previews of scalar arrays: at most eight elements, with an ellipsis when
  the text limit or element limit omits values.
- Pointer addresses, NULL, and the first scalar pointee if its storage fits RAM.
  Double-click navigates to the pointer target, or the variable's own storage.
- Structs, unions, nested arrays and other aggregates identify their byte size and
  storage for RAM inspection. Floating-point scalars show raw bits.

This is a variable inspector, not a general C expression evaluator. Unsupported
DWARF operations, split values, unavailable registers, missing types/locations,
and out-of-range storage have explicit status text. With -O2 a variable may be
optimized away or have no recoverable location at the current PC.

Values reflect described storage **before the next instruction**. The inspector
does not track whether a C initializer has run, object lifetime, or language-level
undefined behavior. A pointer inside RAM is not proof that it refers to a live
object of the displayed type. Caller-saved values are not guessed from the
callee's current registers. Modified function bytes invalidate its local-variable
and unwind metadata when the ELF function range is known.

Source text can be missing while ELF metadata still allows inspection. External
source text read from disk is labeled as unverified against its original build.

## Bounded history

Default capacity is **8192 events**, including instruction attempts, exits, traps
and reversible debugger edits. The CLI accepts 0 through 100000 in RAM; Studio
accepts 0 through 2000000, using preallocated OS-paged disk storage above 100000.
Zero disables recording. Each comparison version has independent storage.
Capacity changes allocate new storage transactionally and clear prior history
without resetting CPU/RAM. An allocation failure retains the old buffer.

Each entry records the before/after CPU, exit status, observed call depth, and the
instruction's memory change. RV32I stores modify at most four bytes, so unchanged
RAM does not need to be copied on each step.

    history
    history 16384
    back 4
    forward 4

The UI and monitor report the actual reserved size from `sizeof(DebugEvent)`,
which depends on the host ABI and the recorded machine/device state. Disk paging
uses the operating system's cache and does not guarantee a fixed resident-RAM
limit. Project files restore the full retained history into fresh local storage.
See [workspace guidance](WORKSPACE_1_2.md) for cache location and cleanup details.

- When full, the ring buffer discards the oldest entry and increments Discarded.
- Back restores the preceding recorded CPU/RAM/exit/depth state.
- Replay restores the following recorded state without rerunning the instruction.
- Step, Run or a source command after Back discards the recorded future before
  appending the newly executed path.
- Reset and capacity changes clear history. Loading replaces the whole session.
- History browsing and breakpoint/configuration changes are not instructions.
  Their UI state is not reversed.
- The History table scrolls through all retained entries using a virtual model;
  the monitor prints up to 16 around its cursor. Use Back/Replay to restore states.
- Reaching the oldest retained entry stops Back; it cannot reconstruct discarded
  instructions. Reset can always restore the loaded initial image.

The current machine has no external input/output services. Its supported ECALL
exit changes only recorded machine state, so it can be reversed. Future
filesystem/device/network services will require recorded results or explicit
irreversible barriers; this implementation does not claim to undo external I/O.

## References

- [DWARF 4 specification](https://dwarfstd.org/doc/DWARF4.pdf)
- [elfutils and libdw](https://sourceware.org/elfutils/)
