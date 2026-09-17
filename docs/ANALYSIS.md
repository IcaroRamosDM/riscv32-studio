# Program Analysis

## Open and navigate

1. Build and load a program, or import ELF/binary/words.
2. Choose **Analysis > Analyze loaded program...** (Ctrl+G).
3. Choose a **Function / entry** in the selector.
4. Use the **Control flow**, **Calls**, **References** and **Pseudocode (experimental)** tabs.

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

### Experimental C-like pseudocode

This view is **off by default**. Enable its checkbox to see a low-level
approximation expressed through 32-bit registers, memory helpers and goto labels.
Click an address label to inspect the corresponding instruction.

It is not the original C or ready-to-compile reconstructed source. It does not
recover original variable names, declarations, types, comments, structured loops,
pointer ownership or function signatures.

The expressions retain explicit machine semantics:

- x1..x31 are 32-bit bit patterns; x0 reads zero.
- wrap32 keeps the low 32 bits.
- i32 interprets the bits as a signed value; asr32 is arithmetic right shift.
- load/store helpers represent checked, naturally aligned little-endian accesses.
- Stores keep the low bits for their width; signed byte/halfword loads extend
  the sign. A load to x0 still accesses memory.
- An indirect jump computes its target before overwriting a link register.
- ECALL, EBREAK and unsupported instructions remain explicit environment/trap operations.

The C analysis core supplies the decoded control flow and instruction lifting.
Qt renders the graph and navigation; it does not execute reconstructed text.

## Analysis method and limits

The analyzer follows code from the entry and known function symbols, adding
conventional call entries as encountered. It follows both conditional successors
and treats the continuation after a call as a potential path.

Direct branch/JAL destinations come from encoded offsets. JALR and data addresses
are resolved only for zero-register bases or short adjacent LUI/AUIPC/ADDI
patterns. An alternative entry invalidates a local pair inference. There is
no general whole-program register-value, pointer, jump-table or stack analysis.
Unknown targets remain unknown.

A speculative call entry can remain listed as an inferred candidate even when
a later discovered alternate entry invalidates the original pair inference.
Raw binaries have no reliable code/data partition or original function metadata.
Their graphs and generated boundaries are therefore conservative approximations,
not a complete reconstruction.

Bounds are explicit:

| Resource | Limit |
| --- | ---: |
| Analyzed instructions | 32768 |
| Function entries | 1024 |
| References | 65536 |
| Code ranges | 256 |
| Address-sorted debug symbols | 262144 |
| Rendered blocks in one control-flow graph | 200 |
| Rendered function entries in the call graph | 120 |
| Visible references per filter | 2000 |
| Pseudocode instructions per function view | 4096 |

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

## References

- [RV32I specification](https://docs.riscv.org/reference/isa/v20260120/unpriv/rv32.html)
- [Qt help events](https://doc.qt.io/qt-6/qhelpevent.html)
- [Ghidra project](https://github.com/NationalSecurityAgency/ghidra)
