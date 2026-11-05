# Compilation Comparison

The comparison workspace compiles one immutable set of source/resource inputs twice.
The defaults are Learning (-O0 -g) and Optimized (-O2 -g); either side can use
-O0, -O1, -O2, -O3, -Og or -Os. The selected GCC or optional Clang compiler and
RV32I/RV32IM ISA apply to both versions, using ILP32.
The versions use the same RAM and reserved-stack sizes. Each has independent
CPU, RAM, source mappings, breakpoint state and execution history.

## Open and use

1. In the main window, choose **Examples > optimization.c**.
2. Leave **RAM=64KiB**, **Stack=4KiB** and **History=8192 steps**, or type your own
   valid capacities. History is allocated separately for each version.
3. Click **Compare -O0 / -O2...**, or choose that action in **Build**.
   Ctrl+Shift+Enter is its shortcut.
4. Wait until the two panels are ready. The original source remains untouched.
5. Set **Compare C line** to **9**, or click that source line. The panels locate
   its first mapped address. The learning build has instructions for the unused
   assignment; the optimized example has none. Inspecting a line does not move
   either machine's PC.
6. Click **Instruction** in the learning panel. Only that CPU advances.
   **Into C**, **Over** and **Out** use the shared debugger's source/call rules.
7. Click **Run both**. Each version runs with its own attempt limit.
8. Both programs exit with status 0. The result symbol contains bytes
   **3b 00 00 00**, the little-endian representation of 59.
9. Click **Back**, **Replay** or **Reset** in just one panel. The other machine
   retains its state.

With the verified GCC 14.2.0 guest toolchain, this example retires **58 instructions
at -O0** and **35 at -O2**, including startup/runtime. The displayed executable
ranges contain 892 and 352 bytes respectively, also including linked runtime.
These are instruction counts and code sizes, not measured hardware speeds.
Compiler versions/options can change the exact numbers.

## Read the comparison

- The shared C text is read-only. Its heading names the panel whose next-PC
  highlight it follows: the last panel controlled by the user.
- During execution, the C line and mapping counts follow that panel's PC.
- Selecting a C line manually shows how many distinct machine addresses map to
  it in each build. It moves the inspection view, not the CPU.
- **Follow PC** returns an Assembly panel to its next instruction. Execution
  controls also resume following their own PC.
- Different PCs, registers and stack layouts are normal at equivalent stages.
  The versions are intentionally not forced into instruction lockstep.
- Double-click an Assembly row to toggle an address breakpoint for that version.
- Each panel provides Registers, RAM, Variables / stack, and History tabs.
  Variable inspection retains the same DWARF limitations as the main workspace.
- Both histories can be browsed completely, up to their configured capacities.
  Back/Replay in one pane do not alter the other.
- Each Run/Into/Over/Out observes its own limit. Pause and Pause both stop between
  instructions. Loops yield to the event loop instead of blocking the interface.

The lower table can compare raw bytes of up to 64 common named data symbols, with
a preview of the first 16 bytes and full-size equality checks. It compares the
current states when both machines are paused, and labels when both have exited.
Pointer bytes may differ because addresses differ; equality is not implied for
all registers or for the full RAM image.

**Choose observations** adds register/global/typed-local expressions and virtual
console, GPIO or exit observations. Unsupported or optimized-out values are
inconclusive. **Record paired checkpoint** captures the selected values at the
two current positions with executable/output hashes. The machines need to be
paused at corresponding semantic points; recording a checkpoint does not align
them automatically. The instruction-differences tab aligns up to 512 instruction
strings per side for the selected C line, without claiming semantic equivalence.

**Run both until condition** (also in the Comparison menu) seeks an intermediate
semantic point automatically. Enter a read-only scalar global/register/memory
condition such as `counter == 10` and a checkpoint name. Each machine pauses at
its first matching state, starting with its current state; the two builds may
execute different instruction counts. The expression is checked on both machines
before either advances. Conditions use the unsigned 32-bit expression rules;
DWARF-local values and the special `$stdout`/`$exit` observations are not condition
inputs. Prefer a meaningful, stable program flag or counter over a scratch register.

The per-version run limit and existing breakpoints/watchpoints remain active.
**Pause both** cancels. Exit, trap, an unavailable expression or another stop
before both conditions are reached pauses the pair and records **Inconclusive**.
The checkpoint includes the condition, limit, per-version attempts, reached flags
and stop details. Successful checkpoints show **Reached on both**; this establishes
only the selected condition and observations, not complete behavioral equivalence.
Saved reports are validated and displayed when reopened, never executed on load.

## What optimization means here

The example computes:

    input = 7
    scaled = input * 8
    adjusted = scaled + 0
    result = adjusted + 3 = 59

The unused local assignment has no observable effect. Optimization can remove
it and remove temporary storage while preserving this program's observable
result. It can also merge operations, reuse registers, reorder work, inline
functions and remove recoverable variable locations.

A missing source mapping alone is not proof of elimination. Debug rows can be
missing, shared, moved or coarse. Even -O0 does not translate one C line into
exactly one instruction. Evaluate the source, actual instructions, available
metadata and observed outputs together.

The example has defined unsigned arithmetic and fixed input. A successful
comparison for one input is selected evidence, not proof that arbitrary C
programs behave identically for all inputs. Undefined C behavior can make such
comparisons misleading.

## Snapshots, builds and limits

The comparison does not replace the main workspace's loaded machine.
**Rebuild snapshot** rebuilds the text already displayed in this comparison.
To compare changed text, return to the main editor and click Compare again.

The main C draft, additional compilation units, runtime inputs, transitive
text/header dependencies and Assembly resources for both optimization profiles
are frozen before either final build. Discovery compiles probes with GNU assembler
dependency tracking, including macro-expanded `.include` and `.incbin` paths.
Clang also uses the GNU assembler so discovery and final assembly agree.
Rebuilding uses the captured bytes, including generated headers/tables already
present at capture time. Binary contents need not be UTF-8. Relative paths and
configured include directories keep resources relocatable; a final build rejects
dependencies that escape the frozen tree. Missing generated inputs must be created
before capture; snapshots do not run or bundle an external build system/toolchain.

Compiler processes run asynchronously. A pair is installed only after both builds
and both loads succeed. Failure or cancellation preserves the previous pair.
Temporary comparison artifacts are retained while their pair is loaded and
removed when replaced or closed. **Save comparison** writes both sessions, frozen
sources, settings, observations and paired checkpoints to `.rv32comparison`.
**Export comparison report** writes readable JSON evidence without changing the
machines. Project persistence saves the main workspace separately.

The C machine and debugger remain unchanged by this presentation layer. The Qt
comparison owns two Session objects, and no registers or RAM arrays are shared.
The UI runs bounded batches; loading and static mapping collection use a worker.

## Related help

- [Desktop workspace](STUDIO.md)
- [Debugger and history](DEBUGGER.md)
- [Source-to-machine execution](C_WORKFLOW.md)
