# Compressed integer instructions (RV32C)

Choose **File > New project from template > Compressed instructions**, then **Build & load**.
The example mixes two-byte and four-byte instructions, calls a function with
`c.jal`, returns with `c.jr`, and exits with 42. Step through it and watch Address,
Encoding, PC and `ra`. Back/Replay preserves the exact instruction boundaries.

The ISA selector offers **RV32IC** and **RV32IMC**, each also available with
**+ CSRs**. The optional C extension applies to the next build/load and persists
in saved projects and frozen comparisons. Machine firmware enables the CSR
dependency while preserving the selected M/C extensions.

```sh
python3 tools/assemble.py --source demos/compressed.asm --arch rv32ic \
  --load-address 0x1002 --output build/compressed
build/debug/riscv32-studio --elf build/compressed.elf --run --json
```

This example returns exit status 42. GCC and optional Clang can also generate
compressed C programs: pass `--arch rv32ic`, `rv32imc`, `rv32ic_zicsr` or
`rv32imc_zicsr` to `tools/build_guest.py`. GNU assembler chooses eligible compact
forms; `.option norvc` locally requests four-byte instructions.

## Inspecting and debugging

Assembly rows follow each decoded instruction's actual width. The instruction
panel identifies that width and explains normalized integer operands. Selecting
an address in the middle of a four-byte instruction does not create an extra
row. **Go** can inspect a different two-byte-aligned entry explicitly, including
self-modifying code. The view lists at most 262144 instructions at once.

The instruction lab decodes a complete 16-bit encoding or a normal 32-bit word.
For compressed instructions it shows the corresponding base operation and the
two-byte link increment. Editing the operand controls produces a base 32-bit
encoding; it does not claim that every edited operand has a compressed form.

Source stepping, breakpoints, source/Assembly mappings, call frames, GDB logical
breakpoints, history, static control flow, code profiles and trace estimates all
use the instruction width. A four-byte instruction crossing an instruction-cache
line contributes accesses to both lines in the educational trace model.

## Files and exact export

ELF loading recognizes the RVC flag and supported ISA attributes. A raw binary,
Intel HEX or S-record has no trusted ISA declaration: select its profile explicitly.
With C enabled, load/entry/PC addresses must be divisible by two. The entry must
contain all two or four bytes of its first instruction. Without C, four-byte
alignment and instruction width remain in effect.

The text `.words` format still supplies exactly four little-endian bytes per
line; one line can contain two compressed instructions. Binary/HEX/S-record are
more natural for odd numbers of halfwords. Assembly export uses exact `.2byte`
and `.word` encodings plus disassembly comments, so rebuilding preserves HINT
encodings and mixed-width byte sequences rather than relying on mnemonic aliases.

## Supported semantics

The decoder implements the 27 integer compressed forms, including `c.nop` and
`c.ebreak`, architectural HINT behavior, signed immediate boundaries and compact
register operands. Reserved encodings, RV64-only forms, custom shift encodings
and compressed floating-point forms are rejected. F/D are separate extensions.

C permits instruction fetch at two-byte boundaries; it does not relax the
alignment of data loads/stores. `c.jal`/`c.jalr` save **PC + 2**. Ordinary JAL/JALR
save **PC + 4**, even when their address is two-byte-aligned. Firmware `mepc`
preserves bit 1 with C enabled; `mtvec` still requires its architectural base
alignment. A failed fetch of the second half of a four-byte instruction reports
that half's address while preserving the original faulting PC.

Primary reference: [RISC-V C extension specification](https://docs.riscv.org/reference/isa/v20260120/unpriv/c-st-ext.html).
