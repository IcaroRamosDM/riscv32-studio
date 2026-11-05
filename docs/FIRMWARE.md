# Machine firmware, interrupts and MMIO

Choose **File > New project from template > Timer interrupts**, then **Build & load**
and **Run**. The handler runs three times, lights the LEDs with 1, 2 and 3,
prints `123` followed by a newline, and returns to the main program with MRET.
Open **Terminal and virtual I/O > Timer / MMIO** and the **CSRs** tab to inspect it.

The **Mode** selector distinguishes **Teaching** from **Machine firmware**.
Firmware enables one to four M-mode harts and requires an ISA with **+ CSRs**.
Selecting it enables that dependency. It applies at the next build/load; the
loaded-profile line identifies the current machine and any changed settings.
Existing projects default to Teaching. A standalone ELF does not identify the
Studio execution environment: select its intended mode before loading it.

```sh
python3 tools/build_guest.py --source demos/c/timer_interrupt.c \
  --arch rv32i_zicsr --environment firmware --output build/timer
build/debug/riscv32-studio --elf build/timer.elf --environment firmware \
  --run --json --observe 'csr(0x342)' --observe 'io32(0xffff0020)'
```

GCC and optional Clang, all optimization profiles, frozen comparisons, Assembly,
projects and batch reports preserve the explicit environment choice. C startup
uses the MMIO exit register after `main`. `rv_stdio.h`, `rv_uart.h` and
`rv_display.h` select their MMIO implementations in firmware builds;
`rv_firmware.h` supplies timer and interrupt helpers. Assembly firmware supplies
its own startup and uses the register map below.

## Experimenting with interrupts

**Schedule timer** sets a 64-bit deadline relative to the current virtual time.
The whole edit is one reversible history entry. The Software/External IRQ
checkboxes set their platform pending levels. Guest delivery also requires the
corresponding `mie` bit and global `mstatus.MIE`. Turning a pending level off or
moving the timer deadline into the future clears its pending condition.

The register table is read-only inspection: it does not consume console/UART
input. **Copy watch expression** creates an `io32(address)` observation with the
same property. **Write selected register** performs an explicit 32-bit device
transaction; it can send a byte, configure a device or exit the firmware.
Invalid transactions preserve device state. Back/Replay reverses/replays both
guest and manual effects; IRQ entries, exceptions and device writes are identified
in the timeline. RAM history never treats an MMIO address as a RAM offset.

Projects can be reopened inside an ISR, after history eviction or with a future
history cursor. They retain timer, CPU/CSR, peripheral, console and manual edit
state. Both comparison machines keep independent clocks. Equal source-level
results do not require equal interrupt timestamps or instruction counts.

## Register map

Every register below is at **0xFFFF0000 + offset** and supports aligned 32-bit
LW/SW only. Other widths, unmapped addresses and writes to read-only registers
raise access faults. This is a Studio platform map, not the address map of a
particular microcontroller or board.

| Offset | Register | Behavior |
|---|---|---|
| 0x00 | LEDs | Read/write low eight bits |
| 0x04 | Buttons | Read current eight input bits |
| 0x08 | Console output | Write low byte; read last byte or 0xFFFFFFFF |
| 0x0C | Console input | Nonblocking read consumes one byte; 0xFFFFFFFF means empty |
| 0xA0 | Console wait | Blocking read; empty input suspends without an attempted instruction |
| 0x10 / 0x14 | Console capacity | Read input available / output free bytes |
| 0x18 | Exit status | Write exits the current hart; all harts exiting finishes the machine |
| 0x1C | Retired instructions | Read low 32 bits of the debugger instruction count |
| 0x20 / 0x24 | mtime | Read/write low / high 32-bit halves |
| 0x28 / 0x2C | mtimecmp | Current hart: read/write low / high deadline halves |
| 0x30 / 0x34 | Software / external IRQ | Current hart: read/write level in bit zero |
| 0x38 | UART status | Read the same flags as `rv_uart_status()` |
| 0x3C | UART data | Read consumes RX; write queues low byte on TX |
| 0x40 | UART configuration | Bits 15:0 ticks/bit, 17:16 parity, bit 18 second stop bit, bit 19 loopback; zero disables |
| 0x44 | UART errors | Write clears error counters; reads zero |
| 0x48 / 0x4C | Display staging | Buffer address / width in low 16 bits, height in high 16 bits |
| 0x50 | Display mode | Write applies staged configuration: 0 off, 1 text, 2 bitmap; read active mode |
| 0x54 / 0x58 | Hart ID / count | Read-only current ID and configured count |
| 0x5C + 16×h | Hart h bank | Deadline low, deadline high, software IRQ, external IRQ, at +0/+4/+8/+12 |

The SDK checks nonblocking UART availability and display/configuration arguments.
Direct stores to a full TX/output queue fail. `rv_timer_at()` uses the safe
low-all-ones, high, low sequence when writing a 64-bit deadline on RV32.
`rv_mtime()` reads high/low/high to handle rollover. Display configuration is
validated atomically; invalid staged values do not replace the active frame.

## Trap and clock model

Exceptions populate `mepc`, `mcause`, `mtval` and transfer to `mtvec`; firmware
ECALL always has machine cause 11, including a7=93. EBREAK uses
cause 3; debugger breakpoints remain debugger stops before execution.
Direct and vectored `mtvec` work; vector offsets apply only to interrupts.
Machine external/software/timer interrupt causes are 11/3/7, in that priority.
Trap entry saves MIE into MPIE and clears MIE. MRET restores MIE from MPIE,
sets MPIE, and transfers to `mepc`; MPP stays M because no lower mode exists.
With [RV32C](COMPRESSED.md), `mepc` retains bit 1 and MRET can return to a two-byte
boundary; without C its low two bits are zero. `mtvec` base alignment is unchanged.

The 64-bit timer advances **one virtual tick per instruction attempt or interrupt
entry**, wraps at 64 bits and pauses with the simulator. Time writes are followed
by that instruction's tick. `time/timeh` mirror it. `mtime >= mtimecmp` posts MTIP
using unsigned comparison. `mcountinhibit` affects CPU cycle/retirement counters,
not the timer. WFI uses the architecturally permitted non-stalling behavior: an
idle loop can repeat it until its interrupt handler changes a shared flag.

The model has no wall-clock or physical CPU timing guarantee, lower privilege
modes, MMU, PMP, delegation, PLIC or operating-system board compatibility.
Unhandled exceptions can repeatedly re-enter an invalid handler until the normal
run limit stops execution. Reconstructed MRET/ECALL operations require explicit
machine-state helpers; they are not silently converted into ordinary C returns.
The pending bit can remain set after a handler disables its `mie` bit; the panel
shows pending and enabled masks separately.

Architecture reference: [RISC-V machine-level ISA](https://docs.riscv.org/reference/isa/v20260120/priv/machine.html).

## RV64 firmware and control registers

Firmware also supports RV64I/IM/IC/IMC with Zicsr, using LP64. Machine CSRs
(`mscratch`, `mtvec`, `mepc`, `mcause`, `mtval`) preserve XLEN-wide values in
instructions, watches, edits, reports, reverse history and projects. The IRQ
flag is bit 63 in RV64; `RV_MCAUSE_INTERRUPT` in `rv_csr.h` selects the right
bit for either XLEN. `rv_csr_*` helpers use `uintptr_t`, and the RV64 counter
helpers read each 64-bit counter directly. RV32-only high-half CSRs and
`mstatush` are unavailable on RV64. M/S/U mode switching is not implemented: the
platform remains M-only, with unsupported status fields fixed to zero.

MMIO registers retain aligned 32-bit transactions at the same addresses. An
RV64 high address does not alias the map; unsupported widths or addresses trap
with the full fault address. Memory capacity remains bounded by the configured
RAM. The timer demo now works in both XLENs. Native C export remains limited
to its separately documented profiles until its runtime is expanded.

Reference: [RISC-V machine-level specification](https://docs.riscv.org/reference/isa/priv/machine.html).

## Per-hart firmware

The **Per-hart timers and software interrupts** template starts four harts with
independent stacks, CSRs, timer deadlines and pending software/external levels.
Each handles one timer interrupt and sends a software interrupt to its neighbor;
the expected total is eight interrupt entries and zero exceptions. Build it with
`--environment firmware --harts 4`, using RV32 or RV64 and Zicsr.

There is one shared `mtime`; every scheduled attempt advances it once, even
when other harts are idle. Every hart sees the same `time` CSR. Cycle/instret and
trap control remain local to each hart. A paused debugger freezes the clock.
The Timer/MMIO panel names the selected hart; Schedule timer and its IRQ
checkboxes target that hart. Explicit MMIO banks can target another existing
hart. Banks beyond the loaded hart count fault. The SDK provides
`rv_hart_timer_at`, `rv_hart_software_interrupt` and `rv_hart_external_interrupt`.

Back/Replay restores shared time and recalculates derived time/pending CSRs for
all harts. Saved projects preserve future/evicted history and the complete
controller state; older single-hart snapshots remain readable. The interrupt
controller is a documented Studio virtual platform, not CLINT/PLIC emulation,
physical timing, operating-system support or RVWMO exploration.
