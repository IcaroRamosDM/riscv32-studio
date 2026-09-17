# Workspace reference

RISC-V Studio connects editable C or Assembly to machine instructions and the
state of an RV32I processor. Use [the complete manual](../HOW_TO_USE.md) for
installation, complete programs and step-by-step lessons.

## Build, load and execute

- **Build** compiles the editor and reports diagnostics. It leaves the current
  execution session unchanged.
- **Load build** initializes RAM and registers from the successful build and
  leaves the new session paused.
- **Run**, **Pause** and **Step instruction** control execution after loading.
- **Reset** restores the initially loaded program and machine state.
- **Compare -O0 / -O2** builds two independent versions of the current C text.

Build results describe a snapshot of the editor. Editing the source after a
build requires a new build before those edits can affect execution.

## Source

The C editor contains the program you can change. The selector distinguishes it
from the compiled snapshot and available startup/runtime/header text.
Loaded source is read-only. It explains where an instruction originated; it is
not a second editable copy of the project.

Selecting a loaded source disables Follow PC so execution does not immediately
replace that selection. Use PC / Follow PC to resume tracking.

See [C compilation](C_WORKFLOW.md) for start.S, the runtime and source mappings.

## Assembly

The PC marker identifies the next instruction. Selecting another row inspects
its decoded operation without changing PC. One C line can cover several
instructions. Startup, compiler helpers and optimized code may have a different
or unavailable C source mapping.

## Registers and Memory

Registers show hexadecimal, signed, unsigned or binary representations.
Changes are highlighted and retain the previous value for inspection.
The Memory panel is a window into allocated RAM; its navigation does not allocate
more memory. Use a symbol, address, Last write or the paging controls to inspect it.

## Inspect, debugger and analysis

Instruction details explain the decoded fields and last step. Other tabs show
build output, symbols, available variables and stack data, breakpoints and
execution history. Back and Replay navigate retained history.

Analysis opens control-flow, call and reference views. Experimental pseudocode
is an explanatory approximation derived from instructions.

## Help and About

F1 and question-mark buttons provide contextual help. The Help window can be
moved, resized and minimized; blank clicks and Escape leave it open.
Its Guides category includes the complete manual, software/hardware explanations,
flowcharts, license and release information.

About is immediately to the right of Context help on the execution toolbar.
It identifies the product version and author, with links to the author's
GitHub and LinkedIn profiles.

## Related guides

- [Debugger and history](DEBUGGER.md)
- [Compilation comparison](COMPARISON.md)
- [Files and projects](FILES.md)
- [Analysis](ANALYSIS.md)
- [Distribution](DISTRIBUTION.md)
