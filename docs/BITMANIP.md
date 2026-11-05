# Bit operations: Zba, Zbb, Zbc and Zbs

Use **File > New project from template > Bit operations** for a complete example.
Build, open Watches and run: `ones=3`, `rotated=259`, `byte_order=0x81000080`,
`polynomial=5`, `selected=0x80000000`, `address=112`. Back and Replay restore
these instructions and memory writes like the other integer operations.

The **ISA** selector includes two profiles with all bit extensions. The adjacent
**…** button opens **Instruction extensions**, where each extension has a short
description. You can enable them independently; the menu retains one custom
profile. Settings affect the next build/load. The loaded machine and saved
history retain their own profile. Machine firmware keeps Zicsr enabled.

| Extension | Instructions | Intended use |
| --- | --- | --- |
| Zba | `sh1add`, `sh2add`, `sh3add` | Shift an index and add an address |
| Zbb | `andn`, `orn`, `xnor`, `clz`, `ctz`, `cpop`, `max`, `maxu`, `min`, `minu`, `sext.b`, `sext.h`, `zext.h`, `rol`, `ror`, `rori`, `orc.b`, `rev8` | Bit counts, rotations, byte operations and min/max |
| Zbc | `clmul`, `clmulh`, `clmulr` | Carry-less multiplication of binary polynomials |
| Zbs | `bclr`, `bclri`, `bext`, `bexti`, `binv`, `binvi`, `bset`, `bseti` | Manipulate one selected bit |

The implementation covers these **32 RV32 instructions**, version 1.0 of each
extension. RV64 word and unsigned-word variants are not part of RV32. Register
bit indices/rotation counts use the low five bits; immediate forms accept 0–31.
CLZ/CTZ return 32 for zero. Signed min/max interpret the same 32-bit register
bits as two's-complement values. Arithmetic wraps to 32 bits. `misa.B` is set
when Zba, Zbb and Zbs are all available; Zbc is independently selectable.

The instruction lab encodes/decodes every operation and disables unused operand
controls. Search Help by mnemonic for a numerical example. C reconstruction
uses unsigned expressions and explicit portable helpers for bit counting,
rotation and polynomial operations; it does not invent original variable types.

For C, `<rv_bits.h>` exposes explicit helpers such as `rv_cpop`, `rv_rol`,
`rv_bset`, `rv_sh2add` and `rv_clmul`. Enable the corresponding extension before
compiling. These helpers have no silent software fallback. Compiler builtins may
also produce extension instructions when their semantics permit it; for example
C `__builtin_clz(0)` remains undefined even though the machine CLZ defines zero.

```sh
python3 tools/build_guest.py --source demos/c/bit_operations.c \
  --arch rv32i_zba_zbb_zbc_zbs --output build/guest/bits
build/debug/riscv32-studio --elf build/guest/bits.elf --run --json
```

ELF attributes determine the loaded profile. Explicitly selecting an incompatible
profile rejects the ELF instead of silently changing its requirements. Raw
binary/HEX/S-record inputs need the intended `--arch` profile supplied by the user.

Architecture references: [RISC-V bit extensions](https://docs.riscv.org/reference/isa/unpriv/b-st-ext.html)
and [machine-level ISA](https://docs.riscv.org/reference/isa/priv/machine.html).

## RV64

The extension dialog also enables Zba/Zbb/Zbc/Zbs independently for RV64.
**New project → 64-bit bit operations** loads `demos/c/rv64_bits.c`.
The 32 common operations use XLEN=64; CLZ/CTZ of zero return 64, full-register
rotations/bit indices use six bits, and immediate variants accept 0–63.
`zext.h` and `rev8` have their RV64 encodings; the distinct RV32 encodings
remain illegal in RV64. Carry-less products preserve both halves before
selecting bits 63:0, 127:64 or 126:63.

The additional RV64 operations are:

- Zba: `add.uw`, `sh1add.uw`, `sh2add.uw`, `sh3add.uw`, `slli.uw`.
  They zero-extend rs1[31:0] before the full-width calculation. The shift in
  `slli.uw` accepts 0–63, after zero extension.
- Zbb: `clzw`, `ctzw`, `cpopw`, `rolw`, `rorw`, `roriw`. They use the low
  input word; rotations use five shift bits and sign-extend the resulting
  word to 64 bits. Counts of a zero word are 32; population count is zero.

`<rv_bits.h>` uses `uintptr_t` arguments/results: unchanged 32-bit API on RV32,
64-bit operands on RV64. RV64 adds explicit `rv_add_uw`, `rv_sh[123]add_uw`,
`rv_clzw`, `rv_ctzw`, `rv_cpopw`, `rv_rolw` and `rv_rorw`. Immediate-only forms
remain available in Assembly and the instruction lab. Existing RV32 examples
that depend on word size should retain their RV32 profile.

These flags compose with the implemented RV64 I/M/A/C/CSR/firmware/integer-vector
profiles. The current machine keeps its architecture while editing the next
build settings; saved history is reconstructed with its original profile.
Runnable C export supports these scalar operations in both XLENs; see `LINKED_C.md`.

Reference: [RISC-V bit manipulation specification, version 1.0](https://docs.riscv.org/reference/isa/v20260120/unpriv/b-st-ext.html).
