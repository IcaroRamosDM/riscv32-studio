# Program Analysis

## Open and navigate

1. Build and load a program, or import ELF/binary/words.
2. Choose **Analysis > Analyze loaded program...** (Ctrl+G).
3. Choose a **Function / entry** in the selector.
4. Use the **Control flow**, **Calls**, **References** and **Reconstructed C** tabs.

Both comparison versions have their own **Analyze** button. Each analysis uses
that version's original executable image. It never advances a CPU, changes RAM,
or substitutes its output for the editable source. RAM modified during execution
does not alter this static view; inspect live bytes in the main Assembly panel.

The window can be moved, resized, minimized and closed independently. Drag a
graph to pan, use Ctrl+wheel to zoom, or choose Fit graph / 100%.

## What each view means

### Control flow

A block groups consecutive instructions without an internal branch target.
Its preview always includes the ending transfer. Longer blocks abbreviate the
middle; double-click a block to inspect its full Assembly.

- Green arrows: a conditional branch is taken.
- Gray arrows: the branch is not taken, or execution continues.
- Purple blocks: calls under the standard RISC-V link-register convention.
- A call's continuation is labeled **if returns**. It does not promise that
  the callee returns.
- Return, stop, unresolved indirect and external targets are identified explicitly.

A graph describes possible static paths. It does not show which branch will be
taken with the current register values and does not prove that every path is
reachable for some input. A target outside analyzed code can be unavailable or
fault during execution.

### Calls

The call graph connects function entries through statically recognized calls
and tail transfers. Double-click a function to select its control-flow view.

Entries reached through resolved calls from the program entry are arranged by
depth. Other linked entries are shown separately: their presence does not mean
they executed. A function can also be reached through an unresolved indirect call.

Functions come from the entry point, available ELF function symbols and inferred
call targets. Names such as sub_00001040 are generated address labels, not
recovered original names. RISC-V mapping symbols such as $x are excluded from
human function labels.

### References

The table connects an instruction to a branch/jump/call destination, a memory
read/write address, or a recognized symbol-address constant.

- **From instruction:** double-click to inspect the instruction.
- **Target / Symbol:** double-click to inspect destination Assembly or data RAM.
- **Evidence:** encoded targets, short register-pattern inferences or unresolved
  runtime registers remain distinguishable.
- **Width:** the memory access width in bytes, where applicable.

Filter by address, symbol, instruction or kind, for example main, 0x00001044,
Call, Read or Write. A known address still needs valid alignment and available
RAM when actually accessed.

### Reconstructed C

The **Reconstructed C** tab is available in both the main Source panel and the
Analysis window. Loading a raw binary or text words selects the main tab;
standalone Assembly selects it after a successful build/load. An ELF with debug
source mappings retains its source-following workflow; select Reconstructed C
to inspect its inferred representation instead.

The reconstruction uses only machine instructions and analysis metadata. It does
not copy the editor, read a neighboring `.c` file or claim to recover original
variable names. It provides:

- Unsigned 32- or 64-bit C arithmetic, folded constants and generated local variables.
- Typed volatile memory accesses and character literals.
- `if/else`, `while`, `do/while` and counted `for` loops for recognized single-entry regions.
- `switch` dispatch for consecutive equality comparisons with one unchanged
  selector. Shared destinations remain shared labels; case values are exact bits.
- Bounded absolute jump-table targets, with a switch on the actual loaded target
  and an explicit fallback helper for targets outside the inferred initial table.
- Entry-register parameter evidence, including address use and signed operations.
  These comments do not assert an original C prototype, pointer type or ABI.
- A string plus an equivalent loop for contiguous constant byte stores, including
  their terminating zero. This can summarize unrolled stores; it does not claim
  that the original program contained a loop.
- Explicit labels or machine operations where structure or behavior is unresolved.

Zve32x/Zve32f instructions remain explicit `rv32_vector` / `rv64_vector` calls
with their Assembly mnemonic. They share vector/FP banks, controls, flags,
reservations and RAM in a machine context. Scalar reads and writes are tracked
separately from similarly numbered vector registers and immediate selectors.
Input evidence marks vector addresses as dynamic; indexed element widths come
from runtime `vtype`, so these inputs suggest `void *` rather than a guessed
fixed-width element type. No array length or native vector ABI is inferred.
Use runnable-project export for the complete checked execution runtime.

Use **Function** to browse a function; this pauses source following. Click **PC**
to resume following the next Assembly instruction. A generated C statement can
represent several instructions, so its highlight can remain on the same line.
Back and Replay use the same mapping. Clicking the gutter, or F9 while this view
has focus, toggles a breakpoint at the first associated instruction address.
Lines such as declarations may have no corresponding instruction.

**Save C...** exports the selected function as UTF-8 C text. It leaves the editor
and machine unchanged. The exported name is generated from the entry address,
for example `fn_00001000`; supply a `main`/startup and appropriate environment if
you want to compile it. Types and input registers are inferred. Memory addresses
are simulated RV32 addresses, not valid pointers to dereference in a normal
Windows/Linux host application. Low absolute addresses can also trigger compiler
diagnostics. Calls and other `rv32_*` operations require manual completion using
the machine model; they are not supplied library functions or recovered APIs.
For an inferred jump table, `rv32_unresolved_jump(target)` must continue the
appropriate machine context or report the unavailable target and return a result.
The generated switch still loads the runtime table: it does not substitute the
initial table's index-to-destination mapping after RAM changes.

If instruction bytes in the selected function change, the original reconstruction
stays visible and its execution highlight is disabled. **Reset** restores the
initial image and mapping. Analysis size limits and unavailable functions are
reported explicitly.

In the Analysis window, **Show instruction-level translation (advanced)** switches
to the older one-instruction-per-line representation for detailed inspection.
It is off by default; readable C is the default view.

### Binary-only Hello World

1. Choose **Examples > hello_world.bin**, or load `demos/binary/hello_world.bin`
   with **File > Load ELF / binary / words...**.
2. For manual loading, select RAM **64KiB**, Load address **0x00001000**,
   Entry PC **0x00001000**.
3. The main **Reconstructed C** tab shows the string `Hello World!`, a byte-copy
   loop to RAM **0x00002000**, and return status zero.
4. In **Memory**, enter **0x00002000** and click **Go**.
5. Step with **F6**, or run with **F5**. The ASCII column displays the message.
   During instruction stepping, its letters appear individually while the
   reconstructed store statement can remain highlighted.

The 120-byte file contains only RV32I instructions, with no C source, ELF header,
symbols or debug data. Its output is memory; it does not call a console service.

## Analysis method and limits

The analyzer follows code from the entry and known function symbols, adding
conventional call entries as encountered. It follows both conditional successors
and treats the continuation after a call as a potential path.

Direct branch/JAL destinations come from encoded offsets. JALR and data addresses
are resolved only for zero-register bases or short adjacent LUI/AUIPC/ADDI
patterns. An alternative entry invalidates a local pair inference. The analyzer
also recognizes an unsigned bounds guard followed by a straight-line affine
address calculation, word load and indirect jump through an absolute-address
table. The table must lie in the initial image and have 1–256 readable entries
with aligned destinations in code ranges. Explicit alternate entries invalidate
that guard. Both `< count` and `<= maximum` guards are supported.

Dashed table edges are hypotheses from the initial image; an unknown runtime
edge remains visible. Relative/PIC tables, arbitrary pointer flows and general
whole-program register/stack analysis remain unsupported. Unknown targets remain
unknown. Later discoveries can invalidate a table after its destinations were
already explored; those instructions remain conservative analysis candidates.

A speculative call entry can remain listed as an inferred candidate even when
a later discovered alternate entry invalidates the original pair inference.
C reconstruction runs a separate fixed-point constant analysis over decoded
function edges. Structure discovery has a bounded work budget; unrecognized
regions retain labels instead of asserting a structure. Functions exceeding
4096 instructions report that reconstruction is unavailable.
Raw binaries have no reliable code/data partition or original function metadata.
Their graphs and generated boundaries are therefore conservative approximations,
not a complete reconstruction.

Bounds are explicit:

| Resource | Limit |
| --- | ---: |
| Analyzed instructions | 32768 |
| Function entries | 1024 |
| References | 65536 |
| Entries in one inferred absolute table | 256 |
| Inferred table destinations, including duplicates | 65536 |
| Table inference scan work | 8000000 items |
| Code ranges | 256 |
| Address-sorted debug symbols | 262144 |
| Rendered blocks in one control-flow graph | 200 |
| Rendered distinct table edges per dispatch | 32 |
| Rendered function entries in the call graph | 120 |
| Visible references per filter | 2000 |
| C reconstruction instructions per function | 4096 |

Reaching an analysis limit is reported as partial results. Large graphs request
table/pseudocode inspection instead of trying to render an unbounded scene.
Analysis uses the initial image, including after self-modifying execution.

## Editor and build workflow

- **Build** (Ctrl+B) compiles or assembles the editor and keeps the current
  machine. It does not load or execute the new result.
- **Load build** (Ctrl+L) installs the most recent matching successful build,
  paused. Source/settings changes disable it until a matching build exists.
- **Run** (F5) executes; **Step instruction** (F6) executes one instruction.
- **Build > Build & load** (Ctrl+Enter) retains the combined shortcut.
- **Build > Save build ELF...** saves a matching compiled artifact before loading.

**Build / output** shows clickable diagnostics with filename, line, column,
severity and explanation. Full compiler output is available separately. A
missing colon after an Assembly label is explained and its actual label line
highlighted. Compare errors open the failed snapshot separately while preserving
the previously loaded pair.

## Understand the source selector

The selector is a browser for the editor and available sources associated with
the loaded executable; it is not a list of editable project files.

| Choice | Purpose |
| --- | --- |
| Edit: filename | Your editable C or Assembly program. Build uses this text. |
| Reconstructed C | Read-only C inferred from loaded machine instructions. |
| Built C / Built Assembly | Read-only copy of the exact text used by the loaded build. |
| Startup: start.S | Read-only startup that prepares the C environment and calls main. |
| Runtime: memory.c | Read-only memory routines linked into the guest program. |
| Header | Read-only type/function declarations from build inputs. |
| Source | Other available source associated with the executable. |

Only **Edit** changes your program. Keeping the built copy separate makes it
possible to inspect what the machine actually received after the editor changes.
Compiler-library source paths recorded in ELF do not imply the source text is
installed. Unavailable files are hidden from the choices; the summary reports
them. Every offered read-only choice opens available text.

Tooltips use one child surface instead of transient native windows. The unsaved
source confirmation stays inside the owner window; blank clicks leave it open,
Cancel preserves the editor, and Discard continues the requested action.

## User address annotations (1.3)

**Names and notes** edits labels and descriptive code/data ranges for the current
image. Graphs and function choices show the user name alongside the original
symbol. Analysis JSON retains the original `name`, adds `userName`, and includes
the validated `userAnnotations` report. Reconstructed C retains its generated
identifiers; annotations are shown as separate escaped notes. These annotations
do not override control-flow discovery or prove that a region is executable.

## References

- [RV32I specification](https://docs.riscv.org/reference/isa/v20260120/unpriv/rv32.html)
- [Qt help events](https://doc.qt.io/qt-6/qhelpevent.html)
- [Ghidra project](https://github.com/NationalSecurityAgency/ghidra)
