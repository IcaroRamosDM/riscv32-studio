# C compilation and startup

## The complete path

1. The editor provides C source text.
2. The RISC-V cross-compiler translates it to RV32I instructions.
3. The assembler and linker produce an ELF executable, including startup code,
   required runtime routines, data sections, symbols and debug information.
4. Load build copies the executable sections into simulated RAM and initializes
   a paused machine.
5. Step instruction or Run executes the loaded instructions.

Build compiles an editor snapshot. It does not save over the user's source file.
A failed build reports diagnostics and does not replace the existing session.

See [How the software works](../HOW_IT_WORKS.md) for the build transaction and
[the complete manual](../HOW_TO_USE.md#3-build-load-and-run) for the exact controls.

## Learning and optimized builds

Learning uses -O0; Optimized uses -O2. Both include debug information.
Learning mode usually preserves source order more closely, but it cannot provide
an exact one-to-one correspondence between C lines and machine instructions.
Optimization can remove expressions, fold constants, inline calls, combine lines
or move instructions.

Compare builds both versions independently. It reports instruction mappings and
statistics for the selected C line. These depend on the compiler version.

## What start.S does

start.S is Assembly passed through the C preprocessor. The uppercase .S suffix
allows preprocessor directives; lowercase .s denotes Assembly without this stage.

The startup routine initializes the stack pointer and global pointer, clears
zero-initialized storage, calls main, and requests an exit using main's result.
This is why execution begins before the first line of main.

runtime/memory.c supplies the guest memcpy, memmove, memset and memcmp routines.
Compiler support routines can implement operations absent from RV32I, such as
integer multiplication/division. The simulator then executes their RV32I code.

## ELF, binary and words

ELF preserves the entry point, loadable sections and available symbols/debug
metadata. A flat binary preserves bytes but needs a load address and entry point.
A word listing is a text representation of instruction/data words.

Export operates on the program image. It is different from saving a project,
which can also preserve the current execution state and editor source.
See [Files and projects](FILES.md).

## Source mappings

Debug line records associate instruction addresses with source locations.
Several instructions may share one C line. A source line can have no mapped
instruction. Compiler-generated code can have no readable original source.

Loaded source is a read-only view of available build sources. The editor snapshot
matches the build text. Other files found on disk may have changed since that
build and are identified accordingly. Missing compiler-library sources are
reported without offering an unusable selector entry.

## Memory layout

The supplied C workflow normally places the program at 0x00001000. This is a
linker choice. The CPU itself does not reserve that origin.

RAM capacity and stack reservation are separate settings. The program, global
data and stack must fit the configured RAM. Increasing RAM changes the accessible
address range; changing the stack reservation changes the space budgeted for
stack use. See [How RISC-V works](../HOW_RISCV_WORKS.md).

## Advanced command-line guest builds

The packaged helper can compile multiple guest source files. This is independent
of the separate terminal simulator project.

On Linux, from the extracted application folder, with the dependencies installed:

~~~bash
python3 tools/build_guest.py --source /path/to/main.c /path/to/helper.c --output /path/to/output/program --ram 64KiB --stack 4KiB --opt 0
~~~

On Windows PowerShell, from the extracted application folder:

~~~powershell
.\python\python.exe .\tools\build_guest.py --source C:\work\main.c C:\work\helper.c --output C:\work\output\program --ram 64KiB --stack 4KiB --opt 0 --cross-prefix .\toolchain\bin\riscv-none-elf-
~~~

Replace the example paths with actual files. Quoted local include directories can
be added with --include-dir. The output prefix produces the ELF, binary and
associated listings/metadata. Open the resulting ELF through
File > Load ELF / binary / words.

Use --help on the helper for its exact options. Guest programs use the supplied
freestanding runtime; this is not a hosted desktop C environment with filesystem,
terminal I/O or operating-system services.

The independent terminal edition is available at
[riscv32-simulator](https://github.com/IcaroRamosDM/riscv32-simulator).
