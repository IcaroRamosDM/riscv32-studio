# Instruction profiles and control/status registers

The default advanced workspace supports the wider pinned ISA catalog. See
[model behavior and controls](ISA_MODEL_LIMITS.md) for pointer masking, timing,
entropy and memory-order properties. The classic profiles below describe the
teaching core; they are not the limit of the default advanced workspace.

Choose the ISA beside the build profile. **RV32I** remains the default.
**RV64I / LP64** and **RV64IM / LP64** add 64-bit integer execution, with optional
M/A/F/D/C, Zba/Zbb/Zbc/Zbs, Zicsr, Zifencei and Zve32x in the extensions dialog; see [RV64](RV64.md) for ABI, debugging and
current profile boundaries.
**RV32IM** enables integer multiplication/division. **RV32I + CSRs** and
**RV32IM + CSRs** add the six Zicsr read/modify/write instructions. The saved
project and frozen comparison retain this choice. **RV32IC/RV32IMC** add compressed
integer instructions, also available with CSRs; see [RV32C](COMPRESSED.md).
The adjacent **…** button opens a scrollable, explained checklist for M, A, F, D, C, Zicsr, Zifencei and
the independently selectable **Zba/Zbb/Zbc/Zbs** bit extensions. Two common bit
profiles are in the menu; one custom profile is retained. A adds 11 atomic word
operations and LR/SC state; see [Atomics](ATOMICS.md) and [Bit operations](BITMANIP.md).
F adds 26 shared binary32 instructions plus four long integer conversions in RV64
and implies Zicsr. In RV32 with C it also enables four compressed floating loads/stores; see [Floating point](FLOATING_POINT.md).
D adds binary64 operations in both XLENs, with NaN boxing for F, and implies F
and Zicsr. C+D adds four compressed 8-byte floating loads/stores. G/GC aliases
expand to IMAFD with Zicsr/Zifencei and optional C.
Choices apply to the next build/load, while the loaded machine retains its ISA.
An ELF may use fewer supported
extensions than the selected profile; incompatible extensions are rejected.
In the CLI, an explicit `--arch` selects the machine's full capabilities after
checking the ELF's minimum requirements. Omit it to infer capabilities from the
ELF. Desktop ELF imports continue to infer their ISA from the file; source
builds record the selected compiler profile in the ELF.

The CLI names are `rv32i`, `rv32im`, `rv32i_zicsr`, `rv32im_zicsr`,
`rv32ic`, `rv32imc`, `rv32ic_zicsr` and `rv32imc_zicsr`. Append any selected
`_zba`, `_zbb`, `_zbc`, `_zbs` in that order after the optional `_zicsr`, for
example `rv32imc_zicsr_zbb_zbs`. Insert `a` after optional `m` and before optional
`f`/`c` for atomics, such as `rv32ia` or `rv32imac_zicsr`. Insert optional `f`
after `m`/`a`, followed by optional `d`, and before `c`; `rv32if` normalizes to `rv32if_zicsr`.
Append optional `_zifencei` last to enable instruction-fetch synchronization,
for example `rv32i_zifencei` or `rv32imafc_zicsr_zba_zbb_zbc_zbs_zifencei`.
Both XLENs support the same scalar extension combinations. F or Zve32x requires
Zicsr; vector profiles explicitly use VLEN128/ELEN32 and the suffix
`_zve32x_zvl128b`. GNU and
Clang versioned ELF attributes are accepted, including Clang's collective
`b1p0` alias for Zba+Zbb+Zbs and the A aliases Zaamo/Zalrsc when A is present.
For C compilation with Clang, redundant Zaamo 1.0/Zalrsc 1.0 Assembly attributes
are folded into their existing A attribute for older GNU assemblers; instruction
bytes, dependency tracking, other attributes and final ELF validation are retained.
Clang's redundant Zcf 1.0 and Zcd 1.0 attributes are folded when their
containing F+C or D+C features are present.
ELF minimum requirements can also name standalone Zmmul 1.0, Zaamo 1.0,
Zalrsc 1.0, Zca 1.0, Zcf 1.0 or Zcd 1.0. The loader maps them to the implemented
containing M, A, C, C+F or C+D features. The selected machine profiles remain the full
features; this does not expose isolated partial-extension profiles.
Guest C uses ILP32 for RV32 and LP64 for RV64; hard-float ABIs remain unsupported.

Example:

```sh
python3 tools/build_guest.py --source demos/c/control_status.c \
  --arch rv32i_zicsr --output build/control-status
build/debug/riscv32-studio --elf build/control-status.elf --run --json \
  --observe 'csr(0x340)'
```

GCC and optional Clang builds, standalone Assembly, exported Assembly and
comparison snapshots use the same accepted profiles. The **Control/status
registers** project template and `runtime/include/rv_csr.h` provide C examples.

## Inspect and edit

`FENCE.I` makes preceding stores visible to subsequent instruction fetches on
the same hart. Studio fetches directly from unified RAM, so no cache invalidation
is needed. The instruction retires without modifying integer registers or RAM;
reserved immediate/rs1/rd fields are ignored as required by Zifencei 2.0.
Without the extension, execution raises an illegal-instruction fault. GNU/Clang,
ELF validation, the instruction lab, saved profiles and reversible execution use
the same selection. `runtime/include/rv_fence.h` provides `rv_fence_i()` for C.
This does not model cross-hart synchronization or physical cache timing.
Linked C retains its explicit stop when fetched code differs from the initial
image; enabling FENCE.I does not remove that export boundary.
See the [RISC-V Zifencei specification](https://docs.riscv.org/reference/isa/unpriv/zifencei.html).

Open the **CSRs** tab beside Instruction, Build/output and Symbols. It lists the
address, name, hexadecimal/decimal value and access policy. Select a row and use
**Copy watch expression** to obtain, for example, `csr(0x340)` for `mscratch`.
These read-only expressions work in watches, breakpoint conditions, exercise
checks, comparison observations and CLI batch observations.

**Edit selected CSR** accepts a 32-bit decimal or hexadecimal value. Supported
writable bits are applied; reserved bits retain supported values. Every edit is
recorded. Back/Replay and project restoration preserve the entire bank, including
the 64-bit counters. Read-only or platform-owned registers cannot be edited here.
Comparison panels show each machine's bank independently.

## Semantics and current boundaries

All six Zicsr forms implement the architectural read/write suppression rules.
An `x0` destination on CSRRW suppresses reading; an `x0` source field on CSRRS or
CSRRC suppresses writing. A different source register containing zero still
requests a write, which is illegal for a read-only CSR. Unsupported CSR addresses
and read-only writes raise an illegal-instruction stop without committing the
instruction's register or CSR changes.

The bank contains `mstatus`, fixed `misa`, `mie`, `mtvec`, fixed `mstatush`,
`mcountinhibit`, `mscratch`, `mepc`, `mcause`, `mtval`, platform-owned `mip`,
64-bit `mcycle`/`minstret` with high halves, read-only `cycle`/`time`/`instret`
aliases with high halves, and zero-valued machine identification registers.
With F it also exposes `fflags`, `frm` and `fcsr`, with mstatus.FS/SD tracking.
This is an M-only bank: MPP stays at M and there are no lower privilege modes.
MRET and WFI are available with Zicsr. Select **Mode > Machine firmware** for
automatic trap delivery, timer/software/external interrupts and MMIO peripherals.
See [Machine firmware](FIRMWARE.md) for its clock, interrupt rules and device map.
Selecting Zicsr alone preserves the Teaching environment's ECALL services.

One attempted instruction advances `cycle` and `time`; only a successfully
retired instruction advances `instret`. The teaching ECALL service does not
retire, so the values can differ. `mcountinhibit.CY/IR` affect cycle/instret;
explicit counter writes take priority over that instruction's implicit increment.
Counters wrap at 64 bits. These are deterministic **instruction-clock counts**,
not wall-clock measurements or a physical microarchitecture's cycle accuracy.
The debugger's lifetime instruction count remains independent of writable CSRs.

Reconstructed C preserves CSR operations through an explicit `rv32_csr` helper
contract with operation/source/read/write arguments. Such exports require a
machine-state implementation of that helper; the tests use a bounded scratch
register model and compare all six forms against the original instructions.

Primary reference: [RISC-V Zicsr specification](https://docs.riscv.org/reference/isa/unpriv/zicsr.html).
