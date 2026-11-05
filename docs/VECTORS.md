# Vectors: Zve32x and Zve32f

The RV32 and RV64 profiles can enable the standard **Zve32x** integer vector extension
with **VLEN=32, 64 or 128, ELEN=32** and Zicsr. Select **RV32IM + Zve32x**, or enable it in
Instruction extensions. Choose the vector register length in that dialog; the default
is 128 bits. Existing scalar extensions may be combined with it.
**Zve32f** adds binary32 floating-point vectors and automatically selects F,
Zve32x and Zicsr. The complete `V` extension is not advertised; `misa.V` remains clear.

## Try it

Create **Integer vectors (Zve32x)** from the project templates, then Build & load.
The C program calls a short Assembly function to add 17 array elements. Step
into `add_vectors`, open **Vectors**, and observe the four-element groups and
the final one-element group. Successful execution exits with zero and leaves
`result=425`, `out[0]=1`, `out[16]=49` and `vl=1`.

The vector panel shows all 32 physical registers as 8-, 16- or 32-bit elements,
with hexadecimal, unsigned, signed, binary or binary32 float formatting. A physical register
contains 4, 8 or 16 bytes according to the selected length. LMUL groups span subsequent physical registers; the
display does not renumber them. Double-click an element, or use Edit selected
element, to change it. Back/Replay undo/restore edits. Changing the display
width or number format does not change the program or `vtype`.
The float format uses 32-bit elements. Its edit dialog accepts decimal floats or
exact bits, including signed zero and NaN payloads. Watches always read raw bits.
Use the expand button to inspect registers in a separate large window. The
Controls dialog explains grouping/masks and offers rounding and saturation edits.

The panel also shows `vl`, `vtype`, `vstart`, fixed-point rounding and saturation.
The mask bit numbered `i` in `v0` controls element `i`. Watches and conditional
breakpoints accept `vreg(register, element, bits)` and `vmask(register, bit)`;
their indices refer to one physical register. `csr(address)` exposes the vector
control registers while the vector context is enabled.

## Execution model

- Integer arithmetic, widening/narrowing, multiply/divide/accumulate, fixed-point
  saturation and rounding, carry/borrow, masks, reductions, permutations,
  extensions and scalar/whole-register moves are implemented.
- Unit-stride, strided, indexed, segmented, mask, whole-register and
  fault-only-first memory operations preserve architectural element order.
  Naturally misaligned accesses trap. Unordered operations use increasing
  element order as a permitted deterministic choice.
- SEW is 8, 16 or 32. LMUL is 1/4, 1/2, 1, 2, 4 or 8, subject to the ELEN/SEW
  constraints. Unsupported configurations set `vill` and zero `vl`.
  `vsetvli`, `vsetivli` and `vsetvl` select `min(AVL,VLMAX)`.
- Tail and inactive elements are retained, including when their policy permits
  agnostic results. Tests against external engines compare only defined bits
  of mask results, whose tails may differ legally.
- `vstart`, `vxsat`, `vxrm`, `vcsr`, `vl`, `vtype` and `vlenb` are exposed.
  `mstatus.VS=Off` rejects vector instructions/CSRs. Writes mark the context
  dirty. Firmware exceptions, interrupts and MRET preserve VS and vector data.
- A partial memory fault retains completed elements and sets `vstart` to the
  faulting element/segment. History includes every attempted access, up to the
  architectural maximum of 128, and reverses overlapping writes in reverse
  transaction order. A fault-only-first load can shorten `vl`.

The instruction inspector lists individual accesses. Memory watchpoints and
GDB watchpoints check every element, including accesses before a later fault.
Retained-history profiles include completed partial accesses. Lifetime memory
profiles count retired instructions; a partial fault marks that coverage
incomplete explicitly. Educational pipeline estimates count data accesses but
do not model vector execution latency or vector dependencies.

## Persistence and integration

Projects save exact vector bytes and all vector CSRs, and verify their state
when restoring/replaying history. Comparison sessions have independent vector
banks and include them in exported reports. The batch JSON includes `vector`
with VLEN/ELEN, control fields and 32 `registersHex` strings in byte order
(lowest-address byte first).

The local GDB server advertises 32-, 64- or 128-bit `v0`–`v31` and vector CSRs. Reads, edits,
reverse execution and element watchpoints use the same machine/history as the
GUI. Read-only CSRs cannot be changed; invalid bulk register packets are
rejected before any state changes.

GCC and Clang builds use ILP32 on RV32 and LP64 on RV64. The prepared
example uses an Assembly function with caller-saved vector registers; it does
not introduce an implicit vector argument ABI. Static analysis and inferred C
now preserve vector instructions as explicit shared-context helpers; integer and
floating register effects are tracked separately. Indexed access widths remain
runtime evidence rather than guessed pointer element types. Runnable C uses the
public vector runtime described in `LINKED_C.md`, including all three VLENs.

[Zve32x](https://github.com/riscv/riscv-isa-manual/blob/main/src/unpriv/zve32x.adoc)
and [vector instruction semantics](https://github.com/riscv/riscv-isa-manual/blob/main/src/unpriv/vector-common.adoc).

## RV64 scalar registers

Zve32x can be selected in RV64 with Zicsr; vector elements remain 8/16/32 bits
and VLEN is selected independently. Scalar AVL, gather/slide offsets and memory addresses use
XLEN. `vmv.x.s` sign-extends to 64 bits; unsuccessful `vfirst` returns XLEN-wide
-1. Negative strides and high-address faults preserve all address bits. `vtype`
uses `vill` at bit 63 in RV64, and unavailable configurations still clear `vl`.

Projects and comparison snapshots preserve that state. GDB describes vector
CSRs with XLEN bits, while physical vector registers use the selected VLEN. CLI JSON
uses a decimal string for RV64 `vtype` to avoid consumer rounding.

## Binary32 floating-point vectors

Create **Floating-point vectors (Zve32f)**, then Build & load. Its C/Assembly
example computes `out[i] = left[i] * 2.0f + right[i]` for 17 elements using fused
arithmetic, then sums each group with an ordered reduction. Successful execution
exits with zero and leaves `result=280.5`, `result_bits=0x438c4000`, `out[0]=0.5`
and `out[16]=32.5`. Both GCC and Clang, RV32/RV64 and Teaching/firmware are supported.

The profile implements binary32 arithmetic, eight fused multiply-add variants,
min/max, comparisons, sign injection, classification, moves, merge, slides,
reductions, integer conversions and the standard seven-bit reciprocal and
reciprocal-square-root estimates. Integer16-to-float32 widening and
float32-to-integer16 narrowing conversions are included; this does not enable
FP16 or FP64 arithmetic. Legal overlapping register groups preserve source data.

Operations share `frm` and accrued `fflags` with scalar F. Use the **Floating
point** tab to select rounding or clear exceptions; the vector Controls dialog
shows their current state. Arithmetic uses SoftFloat; estimate instructions use
the normative lookup tables. All five rounding modes, signed zeros, subnormals,
infinities, NaNs and conversion clipping follow the architectural contract.
Masked, prestart and tail elements neither execute nor raise floating exceptions.
Invalid `frm` values trap even when `vl=0`; FS=Off or VS=Off also rejects execution.

`vfredusum` uses the permitted ordered reduction tree for deterministic results.
An external engine using another legal tree can produce a different rounded sum.
An empty active set copies the reduction seed without canonicalizing its NaN or
raising NV. Reductions require `vstart=0`; `vfmv.f.s` reads element zero even when
`vl=0`. Moves preserve exact bits. Back/Replay and project/comparison snapshots
restore vector bytes, floating registers, flags and FS/VS together. Local GDB
advertises both register banks and supports their combined packets atomically.

See the [official Zve32f profile](https://github.com/riscv/riscv-isa-manual/blob/main/src/unpriv/zve32f.adoc)
and [vector 1.0 semantics](https://github.com/riscvarchive/riscv-v-spec/blob/master/v-spec.adoc).

## Register length and compatibility

The profiles use `_zvl32b`, `_zvl64b` or `_zvl128b` to identify the selected
machine length. An ELF attribute states a *minimum*: a 128-bit machine may load
an ELF requiring 32 bits; a 32-bit machine rejects an ELF requiring 128 bits.
Loading an ELF without an explicit vector selection uses its minimum length.
A vector ELF without a Zvl attribute has the standard 32-bit minimum. Existing
saved machines and vector profiles without an explicit Zvl keep the historical
128-bit selection. Projects preserve the exact choice, including history and
comparison panes. Element sizes remain at most 32 bits; this adds neither ELEN64
nor the complete V extension. Fixed history storage remains unchanged.

QEMU 10.2.1 differs at VLEN32 for segmented accesses, whole-register transfers and some out-of-range gather and slide operations. See
[QEMU whole-register translation](https://github.com/qemu/qemu/blob/v10.2.1/target/riscv/insn_trans/trans_rvv.c.inc),
[QEMU vector helpers](https://github.com/qemu/qemu/blob/v10.2.1/target/riscv/vector_helper.c).

## C export

The exported runtime supports RV32 and RV64 with VLEN32, VLEN64 or VLEN128. See [Runnable C export](LINKED_C.md) for compilation and continuation.
