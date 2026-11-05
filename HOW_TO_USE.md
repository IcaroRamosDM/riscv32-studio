# RISC-V Studio — User manual

The advanced C17/Assembly workspace opens by default. Start with
**Learn by doing** above the editor, or **Help > Tutorials and examples**.
Use [the practical guide](docs/ADVANCED_GUIDE.md), also available
[in Brazilian Portuguese](docs/GUIA_PRATICO.md), for the current build/run
shortcuts, interactive Terminal, examples and layout recovery.
In the default workspace, **View > Panels > History** opens recent records;
**Older**, **Newer** and **Latest** browse that list. **F7/F8** restore the
machine's recorded states. The current position appears above the history table.

The chapters below describe the **classic teaching workspace**, opened with
`--classic` or automatically for older `.rv32project` files. The newer default
workspace uses `.rvstudio` projects.

The [C17 library reference](docs/C_STANDARD_LIBRARY.md) and
[Terminal/panel reference](docs/TERMINAL_AND_WORKSPACE.md) document their own
profile limits. Both interfaces offer English and Brazilian Portuguese labels.
For advanced ISA choices, read [model behavior and controls](docs/ISA_MODEL_LIMITS.md).
The configuration dialog explains the relevant controls for the selected profile.

## Contents

1. [Install and open](#1-install-and-open)
2. [Understand the workspace](#2-understand-the-workspace)
3. [Build, load and run](#3-build-load-and-run)
4. [Files in the Source selector](#4-files-in-the-source-selector)
5. [First complete program](#5-first-complete-program)
6. [Guided lessons](#6-guided-lessons)
7. [Breakpoints and function stepping](#7-breakpoints-and-function-stepping)
8. [Back, Replay and History](#8-back-replay-and-history)
9. [Compare compilations](#9-compare-compilations)
10. [Assembly and RAM boundaries](#10-assembly-and-ram-boundaries)
11. [Import, export and projects](#11-import-export-and-projects)
12. [Static analysis](#12-static-analysis)
13. [Controls and shortcuts](#13-controls-and-shortcuts)
14. [Troubleshooting](#14-troubleshooting)
15. [Limits and glossary](#15-limits-and-glossary)

## 1. Install and open

### Linux 1.9.0-rc1

Download the DEB or portable archive from the
[1.9.0-rc1 prerelease](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.9.0-rc1).
This package targets **Ubuntu 26.04 x86-64 with glibc 2.43 or newer** and a
working graphical desktop. Qt, Python, RISC-V compilers and GDB are bundled.
Debian 12/13 and Ubuntu 24.04 do not meet this package's glibc requirement.

For the DEB, use Ubuntu's package installer or run in the download directory:

```sh
sudo apt install ./RISC-V-Studio-1.9.0-rc1-Linux-amd64.deb
```

Open **RISC-V Studio** from application search, or run
`/opt/riscv32-studio/RISC-V-Studio`. Verify **1.9.0-rc1** in **About**.
To remove the installed package, use `sudo apt remove riscv32-studio`.
Keep personal source files and projects outside the application folder.

For the portable archive, extract the entire folder and execute
`./RISC-V-Studio` inside it. Do not copy only the inner binary. Run
`./install-desktop.sh` once to create an application shortcut; run it again
if you move the folder. `./uninstall-desktop.sh` removes this registration
and preserves the program and user projects.

Before installation, compare your package's `sha256sum` result with its entry
in this release's **SHA256SUMS.txt**. From the extracted portable folder, check
all files and permissions with:

```sh
python/bin/python3 tools/verify_portable.py .
```

Exporting a native host C executable requires
a separate system C compiler and Make.

### Windows and the previous stable release

The [1.0.0 release](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.0.0)
retains its Windows installer, portable ZIP, single portable EXE and original
Linux downloads.
Use the original [1.0.0 installation guide](https://github.com/IcaroRamosDM/riscv32-studio/blob/v1.0.0/HOW_TO_USE.md#1-install-and-open)
for those packages and their version-specific requirements.

## 2. Understand the workspace

| Area | What it shows |
| --- | --- |
| Source | Editable C/Assembly and separate read-only build sources. |
| Assembly | Addresses, encoded instruction words, decoded operations and source locations. |
| Registers | x0-x31, their current values and changes in the latest update. |
| Memory | A 256-byte window into the simulated RAM. |
| Instruction | Decoded fields and meaning of the selected instruction; previous execution effects separately. |
| Build log | Clickable build errors and compiler output. |
| Symbols | Named function/data addresses from the executable. |
| Variables / stack | Available C variables, arguments and call frames while paused. |
| Breakpoints | Enabled address/source stopping points. |
| History | Retained execution events and the current reverse/replay position. |

Drag separators to change panel sizes. Use the horizontal scrollbars for wide
values. Aim for at least 1100 × 780 logical pixels.

The Assembly **>** marker identifies the **next** instruction. Selecting a row
inspects it without executing it. **PC** or **Follow PC** returns to the next
instruction. The highlighted C line can stay unchanged while several Assembly
instructions execute.

**Values** selects Hex, unsigned decimal, signed decimal or binary. These are
different representations of the same bits. The interpretation of comparisons
comes from the instruction, not the display setting.

A star and background color identify register or memory changes in the latest
display update. At high speed an update can contain several instructions.
Hover a register or select a memory byte to inspect the detailed values.
**Last write** navigates to a retained store; it is disabled before any store.

### Help and question marks

Click **?** beside a field for a short explanation. Click it again or outside the
brief panel to dismiss it. **More in Help** opens the full guide.

**F1** opens Help in a separate movable/resizable/minimizable window. Drag its
title bar, use its minimize button and close it with the upper-right **X**.
Blank clicks, Escape and clicking another window leave full Help open.
Search by words such as stack, immediate, signed or jal.

## 3. Build, load and run

These actions have separate effects:

| Action | Result |
| --- | --- |
| Build / Ctrl+B | Compile or assemble the editor into a completed build. |
| Load build / Ctrl+L | Install the matching completed build as a paused machine. |
| Run / F5 | Execute the loaded machine. |
| Build > Build & load / Ctrl+Enter | Build and install in one action, still paused. |
| Build > Save build ELF | Save a completed matching build before loading it. |

Build does not save over your source or start execution. A failed build/load
preserves the previous session. Changing the editor or build settings means the
previous build no longer matches; build again before loading/executing that text.

### Settings

- **RAM:** allocated guest bytes. Type a custom value such as 96KiB.
- **Stack:** reserved part of that RAM for the C stack, for example 6KiB.
- **Build:** Learning (-O0) or Optimized (-O2).
- **Speed:** slow/normal/fast presets or Unlimited. It is not a physical clock.
- **Run limit:** maximum attempted instructions per Run/source-step operation.
- **History:** retained events, from 0 through 100000. Press Enter after typing.
- **Values:** presentation of register and memory numbers.

To edit RAM or Stack, click the text, press Ctrl+A, type the capacity and press
Enter. Their lists are presets, not the only supported values. C builds require
RAM and Stack in multiples of 16 bytes and enough space beyond 0x1000 for the
program and reserved stack. Raw loading permits RAM in multiples of four.

Changing RAM does not resize the already loaded machine. Build and load again
to apply a new C layout. Read the persistent **Loaded** line to distinguish the
loaded profile from newly selected settings.

## 4. Files in the Source selector

This list describes the loaded executable's available source material.
It is not a multi-file editable project tree.

| Choice | What opening it does |
| --- | --- |
| Edit: untitled.c / your filename | Opens your editable program. Build uses this text. |
| Built C / Built Assembly | Opens the exact read-only editor snapshot used in the build. |
| Startup: start.S | Shows the startup instructions linked with C. |
| Runtime: memory.c | Shows the guest memory helper routines. |
| Header: stdint.h, stddef.h, etc. | Shows available declarations/types used while compiling. |
| Source: another available file | Shows other read-only source associated with the executable. |

Choose **Edit** to return to your program. Selecting support files disables
Follow PC so your selection stays visible. Use **PC** when you want to follow
execution again.

start.S initializes the C environment and calls main. The uppercase .S suffix
means preprocessing is performed before assembly. runtime/memory.c contains
memcpy/memmove/memset/memcmp for the guest; it is not the simulator's RAM module.

Unavailable compiler-library files are omitted and counted in the summary.
An ELF path is not an embedded copy of a source file. The program identifies
text read from disk without a verified build snapshot. The same base filename
can legitimately occur in both an editable source and a read-only built copy.

## 5. First complete program

Open **Examples > learning.c**. Its complete source is:

~~~c
#include <stdint.h>

volatile uint32_t result;
uint32_t values[5] = {1, 2, 3, 4, 5};
uint32_t calls;

static uint32_t sum(const uint32_t *items, uint32_t count)
{
    uint32_t total = 0;
    for (uint32_t i = 0; i < count; ++i) {
        if (items[i] > 2)
            total += items[i];
    }
    ++calls;
    return total;
}

int main(void)
{
    uint32_t local[3] = {10, 20, 30};
    result = sum(values, 5) + sum(local, 3);
    return result == 72 && calls == 2 ? 0 : 1;
}
~~~

### Build and follow it

1. Select **RAM 64KiB**, **Stack 4KiB** and **Learning (-O0)**.
2. Click **Build**. Wait for success in **Build log**.
3. Click **Load build**. The machine is paused at startup, normally 0x1000.
4. Press **F6** a few times. Inspect PC and sp in Registers.
5. Press **Into C** (F11) to reach a mapped C location.
6. Use F6 to see the Assembly advance within one C line. Instruction details
   explain each operation.
7. Choose **Unlimited** and press **Run** (F5). Expect **Program exited: 0**.
8. In **Symbols**, double-click **result**. Memory at that symbol starts with
   **48 00 00 00**, the word 72.
9. Double-click **calls**. Its word is 2, bytes **02 00 00 00**.
10. Press **Reset** (Ctrl+R). Initial CPU and RAM return; result/calls are zero
    again, and startup is ready to execute.

The first sum includes 3+4+5 = 12. The local array contributes 10+20+30 = 60.
The final value is 72 and there were two calls. Symbol addresses can differ by
compiler version, so navigate by name rather than copying an address from a
screenshot.

## 6. Guided lessons

For every lesson below: open its entry in **Examples**, choose **Learning (-O0)**,
click **Build**, **Load build**, then **Run** at Unlimited speed. Expect exit
code 0 unless the lesson explicitly creates a fault. Use Symbols to locate
globals and Memory to verify their bytes.

### Signed and unsigned comparisons — decisions.c

~~~c
#include <stdint.h>
volatile int32_t input = -3;
volatile uint32_t signed_less, unsigned_less, result;

int main(void)
{
    signed_less = input < 2;
    unsigned_less = (uint32_t)input < UINT32_C(2);
    if (input < 0)
        result = 100;
    else
        result = 200;
    return signed_less == 1 && unsigned_less == 0 && result == 100 ? 0 : 1;
}
~~~

input is -3: signed_less=1, unsigned_less=0, result=100. The same input bits are 0xFFFFFFFD. Set Values to Signed and Unsigned while paused to see the representations. Inspect the comparison/branch instruction to find which interpretation it uses.

### A loop becomes branches — loops.c

~~~c
#include <stdint.h>
volatile uint32_t limit = 10;
volatile uint32_t result;

int main(void)
{
    uint32_t total = 0;
    for (uint32_t i = 1; i <= limit; ++i)
        total += i;
    result = total;
    return result == 55 ? 0 : 1;
}
~~~

result=55 (0x00000037). Set a source breakpoint on the accumulation line, then Run repeatedly. Watch the counter and the backwards jump in Assembly. Use F6 if several instructions share that C line.

### Calls and recursion — functions.c

~~~c
#include <stdint.h>
volatile uint32_t input = 5, result;

static uint32_t factorial(uint32_t n)
{
    if (n < 2)
        return 1;
    return n * factorial(n - 1);
}

int main(void)
{
    result = factorial(input);
    return result == 120 ? 0 : 1;
}
~~~

result=120 (0x00000078). At -O0, pause inside factorial and inspect Variables / stack. Use Into C to enter a recursive call and Out to return. The RV32I-only target can call software helpers to implement multiplication.

### Adjacent words in memory — arrays.c

~~~c
#include <stdint.h>
uint32_t values[5] = {3, 6, 9, 12, 15};
volatile uint32_t result;

int main(void)
{
    uint32_t total = 0;
    for (uint32_t i = 0; i < 5; ++i) {
        values[i] += 1;
        total += values[i];
    }
    result = total;
    return result == 50 && values[0] == 4 && values[4] == 16 ? 0 : 1;
}
~~~

values becomes {4,7,10,13,16}; result=50. Locate values in Symbols. Its 20 bytes are five little-endian words. Step a store and observe which four bytes change.

### The same storage through word and byte pointers — pointers.c

~~~c
#include <stdint.h>
uint32_t values[3] = {10, 20, 30};
volatile uint32_t result;

int main(void)
{
    uint32_t *p = &values[1];
    *p = 42;
    unsigned char *bytes = (unsigned char *)p;
    bytes[1] = 1;
    result = *p;
    return result == 298 && values[0] == 10 && values[2] == 30 ? 0 : 1;
}
~~~

values[1]=298, hexadecimal 0x0000012A, with bytes 2A 01 00 00. The word store first writes 42. The later byte store changes the second byte to 1. Other array elements stay 10 and 30.

### Watch stack frames grow downward — stack.c

~~~c
#include <stdint.h>
volatile uint32_t lowest_sp = UINT32_MAX, entry_sp, result;

static __attribute__((noinline)) uint32_t descend(uint32_t depth)
{
    volatile uint32_t local[4] = {depth, 2, 3, 4};
    uint32_t sp;
    __asm__ volatile("mv %0, sp" : "=r"(sp));
    if (sp < lowest_sp)
        lowest_sp = sp;
    uint32_t child = depth ? descend(depth - 1) : 0;
    return local[0] + child;
}

int main(void)
{
    __asm__ volatile("mv %0, sp" : "=r"(entry_sp));
    result = descend(4);
    return result == 10 && lowest_sp < entry_sp ? 0 : 1;
}
~~~

result=10 and lowest_sp is below entry_sp. Inspect these named globals after exit, then rerun with a breakpoint inside descend. Watch sp decrease across calls and return as frames unwind. The inline Assembly observes sp; it does not change it.

### Arithmetic implemented with software helpers — arithmetic.c

~~~c
#include <stdint.h>

// Volatile inputs keep these operations visible even in an optimized build.
volatile int32_t left = -12345;
volatile int32_t right = 37;
volatile int32_t product, quotient, remainder;
volatile uint64_t wide_input = UINT64_C(0x123456789);
volatile uint64_t wide_result;

int main(void)
{
    product = left * right;
    quotient = left / right;
    remainder = left % right;
    wide_result = wide_input / 17;
    return product == -456765 && quotient == -333 && remainder == -24 &&
            wide_result == UINT64_C(287454020)
        ? 0
        : 1;
}
~~~

product=-456765, quotient=-333, remainder=-24, wide_result=287454020. The example uses signed division truncating toward zero. Multiplication/division and 64-bit work expand into RV32I instructions and linked helpers.

### Larger RAM use and bounded history — stress.c

~~~c
#include <stdint.h>
volatile uint32_t values[4096];
volatile uint32_t result;

int main(void)
{
    uint32_t total = 0;
    for (uint32_t i = 0; i < 4096; ++i) {
        values[i] = i;
        total += values[i];
    }
    result = total;
    return result == 8386560 ? 0 : 1;
}
~~~

Set Run limit to 2000000 and History to 100000 before building. result=8386560 and values[4095]=4095. Inspect History after exit. With a capacity such as 64 the oldest records are discarded sooner; 0 disables history. Pause remains available during execution.

## 7. Breakpoints and function stepping

1. Open learning.c, select Learning (-O0), Build and Load build.
2. Click the gutter beside **line 22**, the assignment to result. A breakpoint
   marker appears. Check its enabled state in **Breakpoints**.
3. Click Run. Execution stops before entering that mapped source line.
4. Disable the breakpoint temporarily, then choose **Into C** to enter sum.
5. Open **Variables / stack**. Select the current sum frame. Available parameters
   include count=5 and items pointing to the first array.
6. Select main's frame to inspect its available local array.
7. Double-click a readable pointer value to navigate to its target in RAM.
8. Use **Out** to return, then **Over** to complete the remaining call.
9. Continue Run to exit; result is 72 and calls is 2.

Compiler prologues and debug locations affect the first line where a variable
is available. Use another source step after entering the function if needed.
An unavailable value is not zero. Optimized variables can have no recoverable
storage at the selected instruction.

An Assembly breakpoint stops before its address executes. A source breakpoint
stops on entering the mapped line, including a later loop revisit; it does not
stop before every consecutive instruction on that same line.

**Step instruction** deliberately executes one instruction. **Run**, **Into C**,
**Over** and **Out** use their stopping conditions and enabled breakpoints.
Run from a breakpoint passes that occurrence once so execution can continue.
Double-click an Assembly row to toggle its address breakpoint.

Over/Out use observed conventional calls and returns. They do not emulate an
arbitrary debugger for nonstandard handwritten calling conventions. An inlined
function may have no separate call to step over or return from.

## 8. Back, Replay and History

1. Load and execute several instructions of learning.c.
2. Record PC, one changed register and the last written RAM bytes.
3. Click **Back** or press **Alt+Left**.
4. The preceding retained CPU/RAM state is restored.
5. Click **Replay** or press **Alt+Right**. The recorded next state returns.
6. Open **History** to inspect the applied/future boundary.
7. Go Back again and execute a new instruction. The previous recorded future is
   replaced by the new execution path.

History defaults to 8192 events and accepts 0..100000. Type a new value and press
Enter. Changing capacity clears retained history. A full buffer discards the
oldest event; it never grows without limit. You cannot go back before the oldest
retained state. Reset restores the initial program and clears history.

History is distinct from program export. Save a project to retain the machine
and its recorded history. Save source or export ELF does not save current RAM.

## 9. Compare compilations

Open **Examples > optimization.c**:

~~~c
#include <stdint.h>

volatile uint32_t input = 7;
volatile uint32_t result;

__attribute__((noinline)) static uint32_t calculate(uint32_t value)
{
    uint32_t scaled = value * 8u;
    uint32_t unused = value + 123u;
    (void)unused;
    uint32_t adjusted = scaled + 0u;
    return adjusted + 3u;
}

int main(void)
{
    result = calculate(input);
    return result == 59u ? 0 : 1;
}
~~~

1. Keep RAM=64KiB and Stack=4KiB.
2. Click **Compare -O0 / -O2**, or press Ctrl+Shift+Enter.
3. Wait for both sides to finish compiling/loading. The main session remains
   separate.
4. Set **Compare C line** to 9, using the arrow buttons or typed value. Inspect
   whether that line has addresses in each build. This selection does not run
   either processor.
5. Click **Instruction** on the learning side. Only that side advances.
6. Click **Run both**. Both programs should exit with code 0.
7. Inspect result in both sessions: 59, bytes **3B 00 00 00**.
8. Compare the displayed instruction counts and executable sizes.
9. Use Back/Reset on one side and observe that the other retains its state.

The unused assignment has no observable effect and is a useful optimization
example. Exact mappings, instruction counts and code sizes depend on the compiler.
A missing mapping by itself does not prove an operation was removed; inspect
the actual instructions and results.

Each side owns registers, RAM, breakpoints, variables and history. Matching
register values or PCs are not expected at every source point. The shared-source
highlight follows the last controlled side. **Rebuild snapshot** rebuilds the
comparison's captured text. To compare new text, return to the main editor and
open Compare again.

Saving the main project does not serialize the independent comparison pair.

## 10. Assembly and RAM boundaries

Open **Examples > ram_boundary.asm**:

~~~asm
# 64KiB: sw traps because 0x10000 is outside RAM.
# 128KiB: Memory at 0x10000 contains 2A 00 00 00; exit code is 0.
    .option norvc
.text
.globl _start
_start:
    lui t0, 0x10
    addi t1, zero, 42
    sw t1, 0(t0)
    lw a0, 0(t0)
    addi a0, a0, -42
    addi a7, zero, 93
    ecall
~~~

1. Select **RAM=64KiB**, then Build and Load build.
2. Press F6 twice. t0 contains 0x00010000 and t1 contains 42.
3. Press F6 for the store. It faults because 0x00010000 is outside 64 KiB.
4. Set RAM to **128KiB**, Build and Load build again.
5. Run. The store and load succeed, and the program exits with code 0.
6. In Memory, enter **0x00010000** and click Go. Read **2A 00 00 00**.

Only the configured capacity changed. The possible 32-bit address range did not
change. The default program origin 0x1000 is independent from the RAM origin 0.
See [How RISC-V works](HOW_RISCV_WORKS.md) for the address table and last valid word.

### Write a standalone program

Use **File > New standalone Assembly**. Define a global _start label, including
its colon. Use GNU RV32I syntax. The assembler does not add C startup; initialize
sp yourself if your program uses the stack. Stack and C optimization settings
are disabled in this mode.

To deliberately inspect an error, remove the colon from _start:, then Build.
Open Build log and click the diagnostic. Restore the colon and rebuild.
Full compiler output is available separately from the summarized diagnostic.

## 11. Import, export and projects

### Load existing machine code

Choose **File > Load ELF / binary / words** and select a file.

- **ELF:** contains load addresses, entry PC and any symbols/debug information
  that its producer included.
- **Binary:** contains bytes only. Select RV32I and explicitly provide its load
  address and entry PC. For the supplied default builder, both are 0x1000.
- **Words:** use the annotated instruction-word format accepted by the monitor.

Select sufficient RAM before loading. The whole program must fit. Imported raw
bytes cannot identify their own architecture, original C or variable names.
Loading starts paused.

### Export the initial program

Pause, then choose **File > Export loaded program**:

| Format | Use |
| --- | --- |
| Assembly image (.asm) | Reassemblable encoded bytes with readable instruction comments and layout metadata. |
| Raw binary (.bin) | Initial bytes; separately record load address, entry PC and RAM size. |
| ELF (.elf) | Preserve the original ELF and its available metadata; requires an ELF-backed session. |
| Listing (.lst) | Read addresses and disassembly in a text viewer; not assembler input. |

Exports describe the **initial loaded program**, including its original data.
They do not export the current results of execution. Use Save project for the
current machine state.

A complete round trip:

1. Build and Load learning.c.
2. Export ELF, Assembly image and binary to your own folder.
3. Open the exported .asm as source, Build, Load build and Run: expect exit 0.
4. Load the exported ELF: symbols and existing debug data remain available.
5. Load the exported binary with the recorded addresses/capacity. Run: expect
   the same executable behavior, without original C mappings.

The exported Assembly preserves machine bytes. It is not recovered handwritten
Assembly or original C. A file-backed compiler .bin and a full initial-span
export can have different lengths because of zero-filled areas.

### Save and reopen a project

1. Pause your lesson at an interesting point.
2. Press **Ctrl+Alt+S** or choose **File > Save project**.
3. Select a path such as Documents/lesson.rv32project.
4. Close and reopen Studio.
5. Use **File > Open project** (Ctrl+Alt+O), or the installed project association.
6. The main session reopens paused, with the saved editor, machine and history.

A project preserves a draft separately from the compiled snapshot. If they differ,
execution stays disabled until the editor matches again or is rebuilt. Sources
are reconstructed privately; opening a project never overwrites the original
source paths. Use Save source to choose a permanent editable source path.

A project includes available sources, settings and main layout, but not the
compiler, missing files, external build systems or independent Compare sessions.

## 12. Static analysis

1. Load a program.
2. Choose **Analysis > Analyze loaded program** (Ctrl+G).
3. Choose a function/entry.
4. In **Control flow**, inspect the basic blocks and arrows. Double-click a block
   to inspect its instructions.
5. In **Calls**, double-click a function to navigate to its control flow.
6. In **References**, filter by name, address or kind; double-click a source
   instruction or target to navigate.
7. In **Reconstructed C**, inspect the inferred C function. It is shown by default.
   The advanced checkbox switches to the individual-instruction translation.

Drag graphs to pan; Ctrl+wheel zooms. Fit graph returns the view to the graph.
Green arrows represent taken conditional branches; gray arrows represent
continuations. These are static possibilities, not a trace of the current run.

The analyzer reads the initial executable, even after RAM changes during execution.
Unknown indirect targets remain unresolved. Generated C can recover common
control structures but cannot recover original names, comments or declarations.
Large analyses can reach a displayed bound and report partial results.

### 12.1 Reconstruct and step through a raw binary

1. Choose **Examples > hello_world.bin**. This loads the bundled 120-byte RV32I
   program paused, at **0x00001000**, with **64KiB** RAM.
2. Observe the new **Reconstructed C** tab in the Source panel. It shows
   `const char text_1[] = "Hello World!"`, an equivalent loop that writes each
   character to **0x00002000**, and an exit status of zero.
3. Enter **0x00002000** in Memory and click **Go**.
4. Press **F6** repeatedly. Assembly advances every instruction; the reconstructed
   C highlight can stay on one statement for multiple steps. The message appears
   byte by byte in the Memory ASCII column.
5. Use **Back** and **Replay** to inspect earlier/later recorded steps.
6. Choose a different **Function** when a program has several functions. Click
   **PC** to return to execution following.
7. Click a mapped line's gutter, or F9 while this view has focus, to toggle a
   breakpoint at its first associated instruction. Some declarations have no
   associated instruction.
8. Click **Save C...** to save the selected reconstructed function. This does not
   change the editable program or load/run a new executable.

For your own `.bin`, use **File > Load ELF / binary / words...** and supply its
load address and entry PC. Standalone Assembly is reconstructed after Build and
Load build. An ELF with debug mappings can still show available original sources;
select Reconstructed C explicitly to compare the inferred representation.

The generated function needs a startup/`main` and environment before recompiling.
Its memory addresses refer to simulated RAM. Do not interpret those addresses as
ordinary host pointers. Explicit `rv32_*` operations require manual completion;
calls, indirect targets and unusual control flow are not fully decompiled.
See [Analysis](docs/ANALYSIS.md) for the algorithm, example and exact bounds.

## 13. Controls and shortcuts

| Shortcut | Action |
| --- | --- |
| Ctrl+N | New C source. |
| Ctrl+O | Open C/Assembly source. |
| Ctrl+S / Ctrl+Shift+S | Save source / Save source as. |
| Ctrl+Alt+O / Ctrl+Alt+S | Open / Save project. |
| Ctrl+B | Build only. |
| Ctrl+L | Load matching completed build. |
| Ctrl+Enter | Build and load. |
| F5 / Shift+F5 | Run / Pause. |
| F6 | One Assembly instruction. |
| F11 / F10 / Shift+F11 | Into C / Over / Out. |
| F9 | Toggle focused source/instruction breakpoint. |
| Alt+Left / Alt+Right | Back / Replay. |
| Ctrl+R | Reset. |
| Ctrl+Shift+Enter | Compare -O0 / -O2. |
| Ctrl+G | Analyze loaded program. |
| F1 | Contextual offline Help. |

Pause and Cancel build apply to different operations: Pause stops simulated
execution between instructions; Cancel build stops compilation.

## 14. Troubleshooting

| Symptom | Check and next step |
| --- | --- |
| Run is disabled | Build, then Load build. Rebuild if editor/settings changed. |
| Load build is disabled | Wait for a successful matching Build. Check diagnostics. |
| Startup runs before my C | Expected: start.S prepares the runtime. Use Into C or a source breakpoint. |
| C highlight does not advance | Multiple instructions can map to that line. Read Assembly and PC. |
| No C highlight | Check source availability, debug metadata and whether instruction bytes changed. |
| I cannot edit a selected runtime/header file | Choose Edit: your filename. Other entries are read-only build sources. |
| Memory Go does not show 0xFFFFFFFF | Allocated RAM ends at capacity minus one; maximum supported RAM is 256 MiB. |
| Larger RAM did not change the loaded machine | Build/load again. Existing binaries do not rewrite their embedded stack address. |
| Run stops at the limit | Increase Run limit or continue Run. The state is retained. |
| Back cannot go farther | The oldest event was discarded, history is disabled, or history was reset. |
| Variables show unavailable | Try another instruction at -O0. Some DWARF locations/optimized values are not recoverable. |
| Build reports a missing label or opcode | Click the diagnostic; for an Assembly label check the trailing colon. |
| Compiler/Python cannot start on Windows | Extract the complete ZIP; keep its tools/runtime/Python/toolchain files together. |
| Linux cannot find a shared library/compiler | Install via the DEB package manager, or run the archive's dependency installer. |
| Header cannot be found | Save the C file in the directory containing the quoted header, then rebuild. |
| Source diagnostic no longer navigates after editing | Rebuild so diagnostics refer to the current snapshot. |
| Raw binary shows unexpected instructions | Verify architecture, entry/load addresses and byte order; raw files have no metadata. |
| Assembly faults on a load/store | Check the effective address, required alignment and whether every accessed byte fits RAM. |
| Source will not open | Use a regular UTF-8 text file without NUL bytes, up to 4 MiB. |
| Windows reports an operating-system security block | Record the Windows error message and check the installation instructions for your package. |
| An optimization result differs unexpectedly | Check defined C behavior and inputs; compare actual instructions and observable data. |

Building an unsaved editor is supported. Closing/opening another source prompts
for unsaved changes. Save writes your source; Discard continues without saving;
Cancel keeps your current editor.

## 15. Limits and glossary

| Resource | Limit / behavior |
| --- | --- |
| RAM | 4 bytes to 256 MiB; alignment/layout restrictions depend on input type. |
| History | 0..100000 retained events per machine. |
| Breakpoints | Up to 128 per loaded session. |
| Editor/source file | Up to 4 MiB, UTF-8 without NUL. |
| Project file | Up to 128 MiB. |
| Captured project sources | Up to 256 files, 4 MiB each, 16 MiB total. |
| ELF input | Up to 64 MiB. |
| Analysis | Bounded; exact limits are listed in the Analysis guide. |

**PC:** next instruction address. **Register:** 32-bit CPU storage.
**RAM:** byte-addressed guest memory. **Instruction word:** encoded 32-bit operation.
**Immediate:** constant in an instruction. **ELF:** executable container with
addresses and optional metadata. **DWARF:** debug descriptions linking machine
state to source information. **BSS:** zero-initialized global/static storage.
**ABI:** conventions for registers, calls and data layout. **Trap:** a reported
machine/environment exception. **Retired:** completed-instruction count.

For deeper explanations, use [How RISC-V works](HOW_RISCV_WORKS.md),
[How the software works](HOW_IT_WORKS.md), and the application's F1 Help.
