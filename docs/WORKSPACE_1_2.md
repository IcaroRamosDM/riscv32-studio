# Workspace 1.2

This guide includes the incremental 1.3 inspection and comparison additions.

## First steps and interface modes

**Help > First steps** opens an optional guide on the current project. Its buttons
use the same commands as the workspace: build, load, step, inspect registers,
Back/Replay, run/pause and save project. They follow the actual machine state.
Opening or dismissing the guide never replaces source or starts execution. The
welcome banner can be dismissed permanently; Help always reopens the guide.

**View > Interface mode > Essential** starts with workspace tabs, a shorter
execution toolbar and collapsed machine/build settings. **Complete** restores the
previous arrangement. Each mode remembers its docks, splitters and selected tab;
switching preserves the machine, retained history, editor contents and Undo.
All commands remain available in the menus. Preferences are local to this
computer and do not replace project settings.

Build failures now suggest concrete checks for missing compilers/headers,
undefined symbols, RAM/stack overflow, unsupported instructions, promoted
warnings and unwritable folders. The compiler output remains the primary
diagnostic; **Review build settings** opens the existing settings dialog.

Instruction details include the recorded hart and pre-step operands, branch
outcome, effective memory address and CSR changes. These values come from the
recorded event, even after selecting another hart or replaying history. A manual
edit or interrupt before fetch is identified explicitly. Where an operand
snapshot is unavailable, the view says so instead of substituting current state.

In build comparison, **Comparison > Choose observations** selects expressions
and `$stdout`, `$leds`, `$buttons`, `$exit`. Missing values are inconclusive;
console equality uses all output bytes even when the table shows a short preview.
**Instruction differences** aligns exact disassembly text for the selected source
line (up to 512 instructions on each side); highlighted additions/removals do not
imply semantic equivalence. **Record named observation checkpoint** saves the
selected values and positions after you choose meaningful points in both machines.
These are persistent reports with executable/output hashes, not additional
restorable CPU snapshots. **Run both until condition** also advances each version
independently to the first state satisfying a shared global/register/memory
condition such as `counter == 10`, including the starting state. It records a
checkpoint with attempt counts and stop causes; unavailable conditions, early
exit, debugger stops, limits or cancellation before both reach it are inconclusive.
Saved comparisons still contain both complete current machines. Double-click a
checkpoint to review it. Matching conditions do not prove program equivalence.

The application remains an offline educational desktop. RV32I is the default;
select **ISA > RV32IM** to compile native multiply/divide instructions. Profiles
with **+ CSRs** add Zicsr instructions and the **CSRs** inspection/editing tab.
Use `csr(0x340)` in a watch to read `mscratch`; CSR edits participate in Back/Replay.
See [instruction profiles](ARCHITECTURES.md) for the supported register bank,
counter timing and SDK example. Loading an ELF reads its declared supported
architecture. Raw binaries use the selected profile, retained in saved projects.
**Mode > Machine firmware** adds M-only trap delivery, MRET/WFI, reversible
timer/software/external interrupts and MMIO devices; see [Firmware](FIRMWARE.md).
**RV32IC/RV32IMC** add [compressed integer instructions](COMPRESSED.md), including
mixed-width Assembly navigation, two-byte breakpoints and exact exports.
This release does not implement floating point, lower
privilege modes or actual peripheral hardware.

## A comfortable starting layout

Use **View > Compact workspace tabs** (Ctrl+Alt+T) on a small display. Source,
Assembly, Registers, Memory and Inspect each receive the full workspace width.
Disable it for the five-pane view. On first use, short/narrow screens select the
compact layout automatically. Extra project/debug/device panels can be hidden,
docked or floated; compact mode opens them as separate floating panels.

**View > Reading font**, wrapping and zoom adjust source readability. Font,
wrapping, compact mode and the last window/dock arrangement persist locally.
**Ctrl+Shift+P** searches enabled commands. Existing F5/F6 and C stepping shortcuts
continue working. The interface language remains English.

## Sources and compilation

**Tools > Memory search, files and state comparison** separates search/files, regions/accesses and state
differences into their own tabs. The region map describes loaded ELF segments;
segment flags do not impose simulator memory protection. Access profiles can use
retained applied history or all newly executed instructions since reset, including
re-executed paths. Lifetime counts survive history eviction and project saves;
Back, Replay, debugger reads and manual edits do not add to them. Tracking uses
256-byte buckets, at most 65536 distinct buckets, with untracked accesses reported
explicitly. Export writes all tracked buckets even when the display is shortened.
Older projects without these counters are labeled as incomplete coverage.

The same panel reports the observed SP range and, when valid stack-bound symbols
are available, depth from the named stack top and observations below its bottom.
These are execution observations, not proof of stack safety: x2 can also be used
as a general register, and unexecuted paths have not been measured.

**Edit** provides find/replace, line navigation, comment toggling and indentation.
Ctrl+Space offers keywords and words already in the document; it is lexical
completion, without type or scope analysis. Ctrl+Shift+F searches project drafts.

With optional **clangd 21** available, **Edit > C language tools** adds semantic
completion (Ctrl+Alt+Space), definition (F12), references (Shift+F12), formatting
(Ctrl+Shift+I), symbol rename (F2), folding (Ctrl+Shift+[) and unfolding
(Ctrl+Shift+]). The bundled clangd supplies these commands.

Analysis uses the opened drafts, including unsaved headers, and their parsed
dependencies. It does not run a persistent whole-repository background index.
Rename presents current/proposed text before applying; changes are confined to
opened editable documents, remain unsaved and have one Undo operation per file.
Changes made while analysis/review runs invalidate the response. File creation,
removal and renaming requests from the language server are rejected. Operations
can be canceled and have a 30-second response limit.

**File > Add project file** opens extra C/Assembly/header drafts in the Project
files panel. Check compilation units to link them with the main C source. Headers
are reached through includes. Saving a project captures all drafts; saving one
source writes only that source. Each editable file is limited to 4 MiB, with up
to 64 additional editors; build snapshots are limited to 256 files/16 MiB.

Choose -O0, -Og, -O1, -O2, -O3 or -Os. **Build > Build settings** configures local
header directories, C definitions and the local GNU toolchain prefix. Version 1.3
also selects optional Clang for C project compilation; GNU tools perform assembly
and linking, with libgcc providing its runtime.
Executable paths are local preferences; saved comparisons record only the compiler
family, and use local PATH tools when rebuilt after reopening. Standalone Assembly
documents keep using the GNU assembler. A complete successful build precedes machine replacement.

The local object cache verifies dependency/tool/option hashes and each cached
object's bytes. Changed headers and corrupt entries cause recompilation; linking
always runs. Plain Assembly and units with assembler-only resources are freshly
assembled to discover file-search changes as well as changed resource bytes.
The cache is optional in the CLI (`--cache-dir`) and enabled for initialized
desktop workspaces. It is not a guaranteed cross-directory shared build cache.

`program.build.json` records the compiler/components, flags, libgcc, dependency
hashes and ELF hash. **Build > Loaded build fingerprint** shows the attached
metadata; project saves preserve it. Build output reports image span and cache
reuse. This information describes a build, not compatibility with every host.

**Build > Memory sections and stack report** displays ELF allocations and compiler
stack estimates, including values restored from saved projects. GCC/Clang provide
per-function estimates; these are not whole-program peaks and do not include
library/Assembly functions, recursion or full call chains. Dynamic rows require
separate analysis. The gap before the reserved stack is not an allocated heap.
The cache verifies both object and stack-report hashes before reuse.

## Saving and recovery

**Save source** and **Save project** are distinct. Project format 2 stores sources,
initial program, CPU/RAM, debugger, retained history and virtual I/O; readers still
accept format 1. A saved project's machine-state changes trigger an unsaved-state
prompt even if source text is unchanged.

While paused, a recovery copy is written every 20 seconds when changes are
pending. The previous ten recovery versions rotate; named checkpoints are not
automatically removed. **File > Create session checkpoint** and **Restore a
checkpoint** expose those files. **Restore previous session** appears at startup
when a recovery file exists. Saving recovery does not mark edited sources saved.
Storage uses the platform's local application-data directory under `workspace/`.

## Advanced debugging

Set a breakpoint normally and double-click its row to add a condition, ignored
visit count or log message. Conditions use read-only unsigned 32-bit expressions:
registers/ABI names, PC, scalar global symbols, `&symbol`, `u8/u16/u32(address)`,
arithmetic/bit operations and short-circuit `&&`/`||`. Use `slt(a,b)` for a signed
comparison. DWARF locals, structures and arbitrary C expressions are not handled
by this evaluator. Invalid runtime conditions stop with an explanation.

**Debugger > Watches, timeline and profile** contains expressions, RAM access
watchpoints, timeline search, position jumping, logpoints and profiles. A memory
watchpoint stops after the matching completed access. Ctrl+F10 runs to a mapped
source line or selected instruction without installing a permanent breakpoint.

Version 1.3 adds typed watch paths: `row.field`, `matrix[1][2]`,
`pointer->field`, and `local.name` to disambiguate a local variable. Local values
use the selected call frame; registers still refer to the current CPU. Typed
paths are available in watches and monitor `print`, not breakpoint conditions.
**Variables / stack > Explore selected variable** (Shift+Enter) opens an expandable
read-only tree. It supports structures, unions, arrays and pointer targets, with
64 children per branch, 16 levels and 4096 rows. Enter a path for later array
indices. Integer bit fields and complete scalar split locations up to 64 bits
are supported; missing parts and unsupported layouts stay unavailable. See
[the inspector limits](DEBUGGER.md#bit-fields-and-optimized-scalar-locations).
Try `demos/c/typed_inspection.c`; it stops at `ebreak` with initialized examples.
Alt+F11/Alt+F10 travel backward by source line or over a source call.

**Mark position** names a retained state; **Go to marker** restores it. Up to 128
markers persist in the project. Reset, changing history capacity, history
eviction and branching from an earlier state remove affected markers. They do
not extend history capacity or preserve evicted RAM snapshots.

Studio's **History** capacity accepts 0–2000000 events. Up to 100000 uses RAM;
larger capacities reserve a private file on disk and use operating-system paging.
The default remains 8192. Configure the limit before a long run: changing it clears
retained history/markers while preserving the current CPU, RAM, devices and lifetime
profiles. Failure or cancellation preserves the previous buffer and state.
The history footer reports the actual reserved size and storage kind. OS paging
does not impose a fixed resident-RAM quota; the operating system manages caching.

The whole retained ring supports Back, Replay, search, markers, edits and project
save/reopen. Projects contain portable machine/event reconstruction data, not a
native memory dump or a path to an old cache. Each comparison machine has its own
buffer. The automatically refreshed retained profile previews at most the last
100000 applied events and explicitly labels that limit; session lifetime counts
are independent. Once capacity is exhausted, oldest events are still discarded.

Files are reserved under the user's `riscv32-studio/history` cache on writable
disk storage. `RV32_HISTORY_DIR` can select another local directory. RAM-backed
tmpfs/ramfs locations are rejected. Allocation checks reserve 16 MiB of free space
in addition to the file. On Unix the mapped file is unlinked immediately, so the
kernel releases it even after a process crash. On other systems it is removed on
normal close. After a crash on those systems, the file can remain on disk.

**Edit paused machine** records register, PC and memory changes in history. PC
edits clear tracked call depth and exit state; Back restores both. x0 cannot be
edited. History must be enabled. Running/editing from an earlier cursor replaces
the recorded future. Breakpoint visit counts and logpoint visits are execution
control diagnostics; Back/Replay do not rewind them. Reset restarts them and
restored projects start with an empty logpoint message ring.

Compatible source breakpoints survive rebuilds using source mappings and unique
line text. Ambiguous/deleted lines are reported for review. Raw address breakpoints
survive only when the initial memory image is identical. Counters restart on a
new machine. Watchpoint address relocation is not inferred.

## Comparisons, RAM and analysis

The comparison window starts with -O0/-O2 and offers other profile pairs for its
next rebuild. All discovered transitive C headers and Assembly resources for both
profiles are captured before the final builds, including macro-expanded `.incbin`
data and existing generated files. Builds reject dependencies escaping the frozen
tree; use relative resource paths. **Save comparison** stores both independent
machines, history and frozen text/binary inputs.
Use **File > Open saved comparison** in the main window to restore one.

**Tools > Memory search, files and state comparison** searches hex/text/32-bit
patterns, imports and exports selected RAM ranges and compares CPU/register/RAM
with a reference captured in that dialog. Imports must fit RAM and the entire
history capacity; Back reverses the recorded writes. Reference-state differences
do not include device buffers. Memory ranges and access counts are descriptive;
there is no MMU or MPU protection model.

Profiles default to retained events before the current cursor. Select **Count all
executions since reset** to see cumulative visits, including discarded history
and freshly re-executed paths. Back/Replay do not alter these totals. Up to 65536
distinct PCs are tracked; omitted visits are reported. Projects preserve these
counts, and legacy projects explicitly report unknown earlier visits. These are
observed instruction visits, not a percentage of all possible program behavior.

The educational CPU model
states its five-stage/forwarding/cache/predictor assumptions, starts cold at the
retained slice, adds serial penalties and exports its parameters/results. It is
not cycle-accurate and does not predict any particular commercial processor.

Analysis exports evidence as JSON, instruction edges as DOT and the selected
graph as PNG. **Reconstructed C > Export all functions** writes separate C
approximations and a manifest describing helpers/errors. These files are not a
recovered linkable multi-function project; original names/types are not assured.
RV32M arithmetic now reconstructs into self-contained expressions, preserving
division-by-zero and signed-overflow results through explicit widened arithmetic.

## Console, devices and learning

Open **View > Terminal and virtual I/O** or the `console_gpio.c` example. The guest
headers provide `putchar`, `getchar`, `puts` and a small `printf` supporting
`%c %s %d %i %u %x %X %%`. Width, precision, files and floating formatting are not
supported. Empty input returns EOF; input/output capacities are 64 KiB each.

`<rv_stdio.h>` exposes eight LEDs/buttons and a retired-instruction tick count.
These use Studio teaching ECALL services 0x100/0x101/0x110/0x111/0x112, not a
Linux syscall ABI. Queue/button changes and emitted output participate in history.
Reset replays queued input and clears output/LEDs. VCD export includes retained
GPIO changes with a user-selected time scale; the timestamps are not measured
hardware timing. The **Timer / MMIO** tab provides the separate firmware
environment's reversible virtual timer and interrupt controls.

The **Display** tab reads text or monochrome pixels from guest RAM. It provides
PNG/JSON export, textual accessibility descriptions and reversible drawing state.
See [Virtual devices](DEVICES.md) for examples, buffer layout and the guest API.

The **UART** tab shows bit-level TX/RX, bounded queues and error counters. It
supports loopback, queued text/hex input, deliberate parity/stop errors and UART
VCD/Sinalis exports. Its clock advances once per retired simulated instruction.
The device manual describes framing, the nonblocking guest API and history limits.

**Export signal for Sinalis** writes its accepted generic JSON format for one LED,
button or eight-bit value. X is strictly increasing in seconds, generated from
the chosen nanoseconds per retained event; Y uses `logic` or `value`. Open the
JSON as a signal in Sinalis and choose criteria there. Export does not invent a
test verdict, transfer settings to Sinalis or measure physical hardware.

**Tools > Guided exercises** offers eleven tasks, hints, final-result checks and local
completion progress. Checks verify outcomes, not the method used. **Instruction
encoder and data path** encodes/decodes RV32I/RV32M words, explains fields and
common pseudoinstructions, and shows a conceptual data path. It never executes
or modifies the loaded program.

## Batch scenarios and comparisons

The Linux package also includes the command-line monitor. From a source checkout:

```sh
make app
build/debug/riscv32-studio --elf build/guest/program.elf --run --json \
  --console-input input.bin --buttons 85 --observe result --observe 'u32(sp)'
python3 tools/run_scenarios.py --elf learning.elf --elf optimized.elf \
  --cases demos/c/console_gpio.scenarios.json --observe result --output report.json
```

`--json` emits one `riscv-studio-run` result: registers, PC, stop reason, retirement
count, output bytes as hex, GPIO and expression observations. It requires `--run`
and cannot be mixed with trace or program exports. `--arch rv32im` explicitly
enables M for raw images; ELF architecture comes from its metadata and must fit
any explicit restriction.

Scenario files contain `format: "riscv-studio-scenarios"`, `version: 1` and `cases`.
Each case has a name, optional UTF-8 `input` or `inputHex`, `buttons`, and optional
`expect` fields: `exit`, `output`/`outputHex`, `leds` and an `observations` object
mapping expressions to unsigned 32-bit values. Each run starts from a fresh
machine. One to eight ELF snapshots are captured before running, fingerprinted,
and tested against up to 256 inputs. Each run has instruction and host-time limits.

`pass` means complete runs agreed on the selected final observations and supplied
expectations. A mismatch is `fail`; traps, limits, unavailable observations and
host timeouts are `inconclusive`. PC, temporary registers and instruction counts
are intentionally excluded from equivalence comparisons across optimizations.
These finite cases do not prove general equivalence. Report files are created
exclusively, preserving any existing output.

## Addressed firmware imports (1.3)

Use **File → Load executable** to load Intel HEX (`.hex`, `.ihex`) or Motorola
S-record (`.srec`, `.s19`, `.s28`, `.s37`). Select RV32I or RV32IM in the workspace
first: these formats do not describe the instruction set or include DWARF.
Addresses come from the records. The declared entry point is used when present;
otherwise execution starts at the first imported aligned instruction. All four
entry bytes must exist. Unspecified RAM bytes are zero. Saving a project retains
the normalized image, architecture, entry point, current machine and history.

The CLI accepts `--ihex firmware.hex` or `--srec firmware.srec`, optionally
`--arch rv32im` and `--entry 0x1000`. An explicit entry overrides a record's start
address. `--load-address` is invalid for these addressed formats. Every record
must have a valid checksum; overlapping records, inconsistent start addresses,
out-of-RAM spans and missing end records are rejected. A failed import leaves
the workspace's previously loaded session intact. Supported Intel types are
00–05; S-record data types are S1, S2 and S3 with matching termination records.

## Local GDB debugging (1.3)

Load a program, pause, then choose **Tools → Local GDB debugging**. **Start server**
reserves the current machine and selects a free local port unless you supply one.
The dialog displays the executable path for symbols and the exact `target remote`
command. In a separate terminal, start a RISC-V-capable GDB (for example
`gdb-multiarch /path/to/program.elf`) and paste that connection command.

Use `break main`, `continue`, `stepi`, `watch variable`, `rwatch variable`,
`awatch variable`, `print variable` and `set variable variable=42` normally.
`reverse-stepi` and `reverse-continue` use retained history; edits are undone when
crossed. `monitor replay` reapplies one retained event and `monitor reset` restores
the initial machine and clears history. Console/GPIO state follows Back/Replay.
An unavailable history boundary produces an explicit stop/error, not invented
earlier state. GDB watchpoints follow the RISC-V client's before-access convention;
GDB performs its own step-over to show the resulting value.

The server listens only on IPv4 loopback, accepts one client, and never starts
automatically. **Interrupt execution**, GDB Ctrl+C, the per-Continue instruction
limit and client disconnect all pause the machine. Closing the dialog or stopping
the server returns control to Studio without resetting the machine. Studio's
edit/build/execution controls are reserved while the server is listening.

The target exposes 32 integer registers plus PC, one simulated thread, up to 128
logical breakpoints and 32 memory watchpoints. Memory packets are limited to
4096 bytes and edits must fit reversible history. Unsupported packets are reported
as unsupported. No host shell/file service, TLS/network access, extended-remote
process management, physical probe or multi-hart target is provided. The GDB
client is optional and is not bundled. Current integration evidence uses GDB 17.1.

## Address names and notes (1.3)

Pause execution and choose **Tools → Address names and notes**. Select a row to
edit it or use **New** to annotate another range. Enter its start and byte count,
an optional identifier and a classification: note, code or data. **Apply** commits
the form; **Undo** restores up to 32 changes made while this dialog is open.
Ranges must fit RAM and cannot overlap; code ranges use four-byte alignment.

Labels appear in Assembly, symbols, memory tooltips and analysis graphs. The
analysis window's **Names and notes** button starts at its selected function.
Original ELF names remain available; inferred C identifiers and watch-expression
resolution do not change. Classifying bytes as code/data is descriptive metadata
and does not change decoding, access permissions or execution.

Annotations survive Reset, Back/Replay and project saves. Each machine in a
comparison has its own annotations. Loading another image starts a new set.
Export/import uses a JSON report bound to the original image, placement and ISA;
import replaces the set only after validation, and can be undone in the dialog.
There can be 1024 ranges, 64-character ASCII names and 2048 UTF-8 bytes per note,
within a combined 1 MiB metadata limit.

## Compiler diagnostics and code choices (1.3)

**Build → Build settings → Warnings and code** selects common (`-Wall`), extra,
ISO C/pedantic or suppressed warnings. **Stop the build on warnings** controls
`-Werror`; the original pedantic/error behavior remains the default. Syntax and
compiler errors still stop a build when warnings are suppressed.

Optional code choices preserve frame pointers, disable ordinary inlining, keep
tail calls as calls, disable loop unrolling, or enable signed arithmetic wrapping
with `-fwrapv`. The last choice changes the compiler's treatment of signed
addition/subtraction/multiplication overflow; it does not make every C undefined
operation valid. The other choices can help explain generated code, but do not
guarantee complete variable locations or prevent all optimizer transformations.

Options are saved in projects/comparisons, included in build fingerprints and
cache keys, and applied while freezing inputs for both comparison profiles.
They affect C builds, including linked Assembly units; standalone Assembly keeps
its own assembler diagnostics. Changing them requires rebuilding/loading before
continuing a source-linked session. Compiler executable paths stay local.

CLI equivalents are `--warnings=extra --warnings-as-errors=no` and repeatable
`--code-option=frame-pointer`, `no-inline`, `no-tail-calls`, `no-unroll` or
`wrap-overflow`, accepted by `build_guest.py` and `snapshot_inputs.py`.
The [GCC warning](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html),
[optimization](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html) and
[code-generation](https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html) manuals
describe the individual switches; this release verifies the selected switches
with the notebook's GCC and optional Clang.

## Project folders and templates (1.3)

**File → New project from template** previews every file before creating editable
drafts. Choose minimal C, modular C with `src/` and `include/`, console/GPIO,
C calling Assembly, standalone Assembly, RV32M arithmetic, a text display or a
monochrome bitmap display, UART loopback, control/status registers, timer interrupts
or compressed Assembly. Templates also
prepare useful watch expressions and the appropriate architecture. Creation
prepares private working copies, asks about existing unsaved work, then opens a
fresh workspace. **Save project** preserves all files together; it does not require
the temporary originals when reopening or rebuilding on another computer.

**File → Project files and editors** groups the main program and extra drafts by
folder. Filter by filename/path, expand folders with the arrow keys, and press
Enter on a file to open it. The main file remains in the main Source pane.
Checkboxes on additional C/Assembly units control whether they are compiled;
headers are read by includes. Removing a draft never deletes its original file.
**Save file** chooses an explicit destination for the selected extra draft.
Exporting a copy elsewhere preserves the project's original file relationships;
saving the whole project is the way to retain those relationships and all edits.

## Keyboard and editor preferences (1.3)

Open **View → Keyboard and editor preferences** (`Ctrl+,`). Select a command,
record a shortcut and press **Assign**, then **Apply** to save. Conflicting
shortcuts, ambiguous prefixes, ordinary typing keys and reserved clipboard keys
are rejected. **Clear** removes a binding and **Reset keyboard** restores the
defaults. Closing without Apply discards the staged changes. Standard text-field
editing keys remain available. Preferences are local to the notebook, outside
saved projects; project files cannot replace them.

Default **Ctrl+Tab / Ctrl+Shift+Tab** move between source, Assembly, registers,
memory and details, including the compact workspace tabs. **Ctrl+Alt+M** focuses
the menu bar; press Down, then use arrows and Enter to choose commands. F10 is
reserved for **Over**. **Save source as** uses Ctrl+Shift+S and **Create session
checkpoint** uses Ctrl+Alt+Shift+S, removing their former conflict.

**Editor colors** provides Studio dark, higher contrast and light-editor presets,
plus individual source/syntax/gutter/highlight colors and a preview. Text contrast
must be at least 4.5:1 against the specified backgrounds before applying. The
surrounding workspace retains its dark theme. Colors apply to open and newly
created source editors. The editor's accessibility description exposes its next
instruction line, error line and breakpoint count without relying only on color.

## Names in reconstructed C (1.3)

Use **Reconstructed C > Edit names** or **Analyze > Reconstructed C > Edit inferred
C names** to name functions and register-derived arguments/values. A staged table
and C preview show the changes before **Apply**. Blank cells restore generated
identifiers; **Undo last apply** retains the last 32 changes in that dialog.

Names must start with a letter, contain only ASCII letters/digits/underscores and
have at most 64 characters. C keywords, standard integer names, helper names and
collisions are rejected without changing the previous mappings. Renaming replaces
whole identifiers in C tokens, preserving comments, strings and line mappings.
Generated C remains read-only; the original machine image and inferred text are
kept unchanged internally. These user interpretations do not establish original
variable names or types.

Up to 1024 mappings are saved with each project machine, retained by Reset and
used by the main reconstructed view, analysis view and individual/all-function C
exports. The export manifest and analysis JSON include the original identifiers
and user mappings. Function filenames include their addresses. Separate function
exports still do not constitute a recovered linkable project.

## Stack and pointer map (1.3)

Pause a loaded machine and use **Tools > Stack and pointer map** (also available
on each comparison pane). Select a described call frame to see its variables:
solid arrows mean “points to”; dashed arrows mean “contains”. Repeated pointer
targets share nodes, so linked lists with cycles cannot cause unbounded traversal.
The **Values** table exposes the same names, types, values and addresses for keyboard
navigation. **RAM around SP** shows little-endian words without interpreting them
as live variables. Select an address and use **Show selected address in RAM**.

The map shows at most 32 roots, 96 values, 8 children per value and 4 expansion
levels, with a visible partial-map message. Unavailable optimized/debug locations,
null pointers and targets outside RAM stay explicit. SP/CFA intervals describe
available debug observations; they do not prove ownership or object lifetimes.
JSON contains the complete bounded snapshot; PNG exports are limited to 4096
pixels per side. Inspecting and exporting do not execute or modify the machine.

The **Stack watch** tab optionally pauses after an instruction changes SP outside
an inclusive range or violates the selected 1/4/16-byte alignment. ELF stack
symbols can supply the boundaries. Initial SP equal to the top of RAM is valid.
Resume continues from the recorded result; Back/Replay do not generate new alerts,
and another offending SP change alerts again. Existing breakpoint/watchpoint
reasons are preserved. Reset keeps the configuration, and projects save it per
machine. This is a debugger aid, disabled by default; it does not enforce memory
permissions or certify safe stack use.

## Project exercises and local reports (1.3)

**Tools → Project exercise and reports** attaches an assignment to the current
project. In **Set up checks**, enter a title, instructions, hint and up to 32 named
expression/value checks. Expected values are unsigned decimal or hexadecimal
32-bit patterns; use `0xffffffff` for the bits representing signed -1. Expressions
can inspect registers, scalar globals or `u8/u16/u32(address)`. `$exit`, `$leds`,
`$buttons` and `$output_size` expose the corresponding virtual-machine values.
An optional exact console check accepts hexadecimal bytes, including NUL/newline.

**Apply exercise to project** prepares the applicable watch expressions. **Require
the program to exit** is on by default; turn it off for an intermediate paused
state. **Import checks** fills the form for review; **Export checks** saves a JSON
assignment. Neither evaluates expressions or runs the program automatically.

In **Exercise**, write an explanation and choose **Check current result** while
paused. **pass** means the selected observations matched; **fail** means at least
one available observation differed. Missing values, absent/outdated executables
or unmet completion requirements produce **inconclusive**. Checking leaves CPU,
RAM and debugger history unchanged. It records actual values, explanations, PC,
retirement count, image/source hashes and a timestamp. It does not establish
correctness for untested inputs or assess the solution method.

**Saved reports** retains up to 64 reports within 1 MiB, discarding the oldest
when needed. Export a selected report as HTML for reading or JSON for processing.
Each report keeps its own assignment, so older results remain identifiable after
changing the current checks. **Save project** stores the assignment and reports
along with sources, executable and paused machine. Reopening never automatically
runs checks. Guided exercises use this same persistence/reporting path; the
console/LED exercise checks output and LEDs as well as its result variable.

Assignment JSON uses `format: "riscv-studio-exercise"`, `version: 2`, `title`,
`instructions`, `hint`, boolean `requireExit` and a `checks` array with `name`,
`expression`, and string `expected`. Optional `outputHex` checks exact console
bytes. Limits are 64 KiB per assignment, 512 UTF-8 bytes per expression, 16 KiB
instructions/notes, 8 KiB hint and 8192 expected console bytes. Imported project
reports are bounded and checked for consistent results before replacing a session.

### Advanced guided activities and wide reports

The exercise picker previews instructions and starting source before loading.
Loading explicitly selects the displayed ISA/environment/hart count, 64 KiB RAM,
4 KiB stack per hart and -O0; unsaved changes keep their normal cancellation guard.
New tasks cover timer interrupts, UART loopback, vector masks, a four-hart shared
counter and -O0/-O2 comparison. Each starts with a deliberate error. Checks observe
final values; the comparison activity additionally asks you to run both builds.
The counter exercise illustrates deterministic interleaving, not all weak-memory
behaviors. Local progress uses stable activity identifiers across languages.

Exercise version 2 accepts unsigned expected values/hex patterns up to 64 bits.
Version 1 keeps its original 32-bit expected range. New reports (version 2) retain
architecture, selected hart and hart count, and reopen for every supported profile.
Existing version 1 reports remain readable. Negative bit patterns must match the
observed value width. Reports record outcomes, not proof of every possible input.

### RV64 compressed instructions

RV64 now supports C alongside I/M/Zifencei, with LP64. The instruction inspector,
encoder/decoder, ELF/binary loaders, reverse history and projects distinguish
RV64C encodings from RV32C. RV64C includes C.LD/C.SD, stack forms, C.ADDIW,
C.ADDW/C.SUBW and 0–63 shifts. Mixed 16/32-bit code is fetched at two-byte
boundaries; a compact jump writes PC+2. Reserved encodings remain illegal.
Double-precision floating point is not enabled by C alone.
