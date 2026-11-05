# Floating point (RV32/RV64 F and D)

Enable **F** in Instruction extensions and Build & load. Zicsr is selected
automatically because F uses its rounding/exception CSRs. The **Floating point**
tab shows all 32 floating registers as decimal values, exact bits and IEEE classes.
With F they are 32 bits; with D they are 64 bits and narrower F results are NaN-boxed.
Unlike x0, f0 is writable. The Floating-point arithmetic project template provides
a prepared C example, including a fused calculation whose result would change
with intermediate rounding.

The core supports all 26 RV32F instructions, the five standard rounding modes,
static/dynamic rounding, subnormals, signed zeros, canonical arithmetic NaNs,
payload-preserving moves/sign injection and precise aligned memory accesses.
In RV32 with C, C.FLW/C.FSW and their stack forms are also available. RV64C
uses those encodings for integer doubleword loads/stores. FMA rounds once.
NaN-to-integer conversions and overflow clip according to the RISC-V specification.
Floating exception flags accumulate; they do not raise arithmetic traps.

**Dynamic rounding** controls frm. An instruction with an explicit rounding mode
overrides it. Reserved modes cause an illegal-instruction exception. **Clear flags**
clears NX (inexact), UF (underflow), OF (overflow), DZ (division by zero) and NV
(invalid operation). The CSRs tab also exposes fflags, frm and fcsr.

The platform initializes mstatus.FS to Initial when F is selected. Firmware may
disable the context by writing FS=Off; floating instructions and floating CSRs
then trap. Writes to floating state mark FS=Dirty and set SD; traps/MRET preserve
FS. Manual register editing deliberately enables a dirty floating context.

**Edit selected value** previews decimal input or exact hexadecimal bits. Use the
hexadecimal option to create a particular NaN payload. Back/Replay restores the
register bank, flags, FS and RAM together; saved projects and independent
comparison machines retain the same state. `freg(0)` watches raw bits and
`fclass(0)` returns the standard ten-bit classification mask. `csr(1)` reads flags
when FS is enabled. The instruction lab exposes the third FMA operand and rounding.
GDB receives the floating feature, register values and editable floating CSRs.

C compilation uses **ILP32 for RV32 and LP64 for RV64**, even with F.
Ordinary `float` arithmetic uses F; SDK `rv_float.h` adds flags, rounding, sqrt,
fused multiply-add, classification and bit conversions. Do not enable compiler
fast-math assumptions when observing exception/rounding behavior. The bundled C17 library formats and scans `float`, `double` and `long double`.
F-only profiles execute double-precision arithmetic in software. Select D for
binary64 instructions. Hard-float ABIs are a separate pending integration.
Enable the separate [Zve32f vector profile](VECTORS.md) for binary32 vector operations.

Execution uses the pinned Berkeley SoftFloat 3e binary32/binary64 subsets, with per-thread
state; host floating point is used only for decimal display/input. License and
source fingerprints are under `third_party/softfloat/`; packages include its BSD
license. Reconstruction keeps explicit shared-machine floating helpers instead
of claiming to recover original C types or replace IEEE behavior with host math.

## RV64F

Select RV64 and F in Instruction extensions; Zicsr is selected automatically.
**New project → Floating point with 64-bit integers** demonstrates conversions
and their flags. Integer registers/addresses are 64 bits and floating registers
remain 32 bits (FLEN=32). All 26 shared F operations are available, plus
`fcvt.l.s`, `fcvt.lu.s`, `fcvt.s.l` and `fcvt.s.lu` for signed/unsigned 64-bit
integers. `rv_float.h` exposes `rv_float_to_i64`, `rv_float_to_u64`,
`rv_float_from_i64` and `rv_float_from_u64`, using the selected dynamic rounding.

`fmv.x.w`, `fcvt.w.s` and even `fcvt.wu.s` sign-extend their 32-bit result to
XLEN. `fmv.w.x` and conversions from W/WU use only the low 32 bits. FLW/FSW
preserve exact bits and use full 64-bit addresses; invalid high addresses never
alias low RAM or MMIO. Floating state, FS/SD and flags are per hart, shared by
Teaching/firmware execution, reversible edits and project/comparison saving.
Select D for double-precision instructions. Hard-float calling conventions remain
a separate pending integration.
The runnable C backend supports scalar F in both XLENs, including the four
RV64 integer conversions and complete FS/SD/exception state.

The contract follows the [RISC-V F specification](https://docs.riscv.org/reference/isa/v20260120/unpriv/f-st-ext.html).

## D and mixed precision

Enable D in Instruction extensions, or select RV32G/RV32GC/RV64G/RV64GC.
D implies F and Zicsr; G abbreviates IMAFD with Zicsr and Zifencei. CLI/build
tools accept these G/GC aliases and record the canonical architecture.
**New project → Double precision floating point** builds `double_precision.c`.
`runtime/include/rv_double.h` provides exact bit conversions, classification,
sqrt, fused operations and integer conversions using the selected rounding mode.

D adds 26 operations in either XLEN and six RV64-only long-integer conversions
and raw 64-bit moves. FLD/FSD and C.FLD/C.FSD/C.FLDSP/C.FSDSP transfer aligned
8-byte values in both XLENs. Addresses retain the machine's XLEN; FLEN and XLEN
are independent. F instructions validate NaN boxes except raw FMV.X.W and FSW
transfers. Arithmetic treats invalid boxes as canonical quiet NaNs. Narrow
results have all upper 32 bits set; loads and raw moves preserve payloads.

The panel defaults to binary64 for D, with a precision selector to inspect the
binary32 interpretation and its NaN box. Hexadecimal editing accepts the complete
64-bit pattern; narrow decimal edits create valid boxes. GDB describes and transfers
64-bit FP registers independently of integer width. Back/Replay, project restoration,
per-hart state and generated C preserve all 64 bits. Batch JSON version 4 writes
FLEN64 registers as decimal strings. `freg`, `u64` and 8-byte symbol observations
retain 64-bit values even in RV32, including arithmetic around those operands.
