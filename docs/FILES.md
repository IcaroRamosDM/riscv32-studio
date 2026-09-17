# Files, Export and Projects

## Choose the right format

| Format | What it preserves | How to open it |
| --- | --- | --- |
| C source (.c) | Editable program text; local headers remain build inputs. | File > Open C / Assembly source, then Build and Load build. |
| Assembly source (.asm, .s, .S) | Instructions/directives and labels written in the source. | Open source, then Build and Load build. Uppercase .S runs the C preprocessor. |
| ELF (.elf) | Initial image, segment addresses, entry, and any symbols/DWARF included by its producer. | File > Load ELF / binary / words. |
| Raw binary (.bin) | Initial bytes only. | Load program; explicitly enter load address and entry. |
| Assembly image (.asm export) | Exact initial bytes, entry, load address and minimum RAM metadata. | Open source, then Build and Load build. |
| Disassembly listing (.lst) | Addresses, instruction encodings and readable disassembly for inspection. | Open in a text viewer; it is not assembler input. |
| Project (.rv32project) | Editor, loaded program, available sources, machine/debug state, settings and main workspace. | File > Open project (Ctrl+Alt+O). |

The machine is fixed to RV32I/ILP32, 32-bit little endian. The raw-load dialog
states this architecture explicitly. The CLI accepts --arch rv32i and rejects
other architectures. A raw file cannot identify its own architecture or addresses.

Loading Assembly, ELF or binary exposes decoded instructions in the Assembly
panel. It does not recover the original C text, names or comments. The analyzer provides optional
register-level C-like pseudocode, not recovery of the original C; see [Program Analysis](ANALYSIS.md).

## Import standalone Assembly

1. Choose **File > New standalone Assembly**, or **Open C / Assembly source...**.
2. Define a global **_start** label. Choose RAM, click **Build**, then **Load build**.
3. Use the same Step, Run, registers and Memory controls as for C.

The standalone assembler does not insert the C runtime or initialize sp/gp.
Set up a stack in the source if it needs one. Stack size and C optimization
controls are disabled for this mode. Compare is available for C only.

The default load address is 0x1000. An exported Assembly image carries its own
load-address metadata; its recorded minimum RAM must fit the selected capacity.
Assembly uses GNU syntax, RV32I and the ILP32 ABI. Unsupported instruction
extensions are rejected.

For a complete standalone example, open Examples > ram_boundary.asm.
A failed assembly preserves the existing loaded session and successful build.

## Export the loaded program

Pause execution, then choose **File > Export loaded program**:

- **Assembly image (.asm)** writes encoded .word/.byte directives with readable
  instruction comments, plus zero-filled regions and address/RAM metadata.
  It can be assembled again with the supplied importer without changing bytes.
- **Raw binary (.bin)** writes the initial loaded span, including zero-filled
  BSS, stack and gaps in that span. Record the load address, entry and RAM size
  reported after export; the file itself cannot store them.
- **ELF with debug data (.elf)** saves the original loaded ELF, including whatever
  metadata it contains. This option requires an ELF-backed session.
- **Disassembly listing (.lst)** is a readable listing, not reassemblable source.
  Data bytes can decode as instructions too; a listing does not establish which
  words the program will execute.

Exports use the **initial program image**, even after the program modifies RAM.
Use a project file to preserve the current CPU/RAM state and debugging session.
The assembly image preserves encodings and addresses, not the original C,
symbol table or DWARF. Keep an ELF or project when those are needed.
An ELF with no debug information remains without it after export.

File writes are atomic: a rejected export preserves the existing destination.
Choose paths different from the input source and executable.

The guest compiler's .lst is an annotated objdump listing. Its .bin contains
file-backed sections; the monitor/desktop binary export includes the full
initial loaded span. Both are flat bytes, but their lengths can differ.

## Save and reopen a project

1. Pause the machine.
2. Choose **File > Save project...** (Ctrl+Alt+S), for example lesson.rv32project.
3. Use **File > Open project...** (Ctrl+Alt+O) to resume it later.

A project stores:

- Editable C/Assembly text, including an uncompiled draft.
- The loaded executable separately from the editor, so edits made after the
  last build remain distinct from the code the machine was executing.
- Available build-source snapshots, local quoted headers, runtime sources and
  text Assembly includes, within the bounds below.
- Initial image, current registers/RAM/PC, halt/exit state and debugger call depth.
- Address/source breakpoints and enabled flags.
- Retained history, its capacity, cursor, recorded future and discarded count.
- RAM/Stack/build/speed/value/run-limit settings and main-window size, splitters,
  tabs, source selection, editor cursor and memory navigation.

Opening restores a paused session; it does not start execution automatically.
Reset restores the saved initial image. Back and Replay continue within the
retained history. A project also preserves an editor that differs from the
loaded snapshot; execution stays disabled until that source is rebuilt or undone.

Available source files are reconstructed in private temporary storage.
Opening never overwrites the original source paths. Save source from an opened
project asks for an explicit destination; Save project preserves it in the
project archive. Rebuilding needs the installed compiler and this application's
tools/runtime. Projects do not bundle a compiler, arbitrary build systems,
binary .incbin assets, or missing source files.

The independent Compare window is not serialized. Recreate its two sessions
with Compare from the saved C editor. Help window position is retained while
the application is open, separately from the saved main workspace.

## Validation and bounds

The loader checks the project format/version, architecture, file paths, sizes,
checksums, machine state and restored history before replacing the current
session. Malformed projects leave that session intact.

- Project file: at most 128 MiB.
- RAM: at most 256 MiB; ELF: at most 64 MiB.
- Saved text sources: at most 256 files, 4 MiB per file and 16 MiB total.
- Editor/source input: at most 4 MiB of UTF-8 without NUL bytes.
- History: at most 100000 retained events.

Very large encoded Assembly exports may exceed the source-import text limit;
use ELF/binary or a project for those images. Saving/loading large memory and
history snapshots can take longer than small lessons.

## Why 0x1000, and what does more RAM change?

RAM itself begins at **0x00000000**. The supplied C linker script places the
program at **0x00001000** (4096), leaving the first 4 KiB outside its linked
sections. This is a layout choice, not a requirement of RV32I. Those low bytes
are still accessible RAM: there is no protected null page in this simulator.

| RAM capacity | Valid byte addresses |
| --- | --- |
| 64 KiB | 0x00000000 through 0x0000FFFF |
| 128 KiB | 0x00000000 through 0x0001FFFF |
| 1 MiB | 0x00000000 through 0x000FFFFF |
| 256 MiB (current maximum) | 0x00000000 through 0x0FFFFFFF |

A 32-bit address can name 0xFFFFFFFF, but that does not allocate 4 GiB of RAM.
The highest usable byte here is **RAM size minus one**. Every byte of a load or
store must fit: in 64 KiB, the last aligned four-byte word starts at 0x0000FFFC.

Increasing RAM makes more addresses valid and gives programs room for more
code, globals, arrays and stack. It does not make each instruction faster.
For C, rebuilding also moves the stack top to the new RAM end: the initial sp
is 0x10000 with 64 KiB and 0x20000 with 128 KiB. sp initially points one byte
past RAM, then decreases before stack stores.

Changing the RAM field applies to the next successful build/load. It does not
resize the running machine. Reloading an existing binary with more RAM does not
rewrite its instructions or relocate an embedded stack address; rebuild C to
link against the new layout.

### See the execution difference

1. Choose **Examples > ram_boundary.asm**.
2. Set **RAM = 64KiB**, click **Build**, then **Load build**, then **Run**.
   The store at address **0x00010000** fails because it is outside RAM.
3. Set **RAM = 128KiB**, then **Build**, **Load build** and **Run** again.
   The same store and load succeed; the program exits with status zero.
4. In **Memory**, enter **0x00010000** and click **Go**.
   The first bytes are **2A 00 00 00**, the little-endian word 42.

Memory shows a 256-byte window into actual allocated RAM. Its address navigation
does not create memory or make an out-of-range access valid.

## Help window

F1 opens a separate, movable, resizable and minimizable window. Drag its title
bar to place it, use the title-bar minimize button, and use the upper-right
**X** to close it. Blank clicks, clicks outside, Escape and loss of focus leave
it open. Reopening preserves its placement and cached content during the session.
