# Virtual devices

The Studio execution environment provides deterministic teaching devices. Device
configuration and guest RAM stores participate in Back/Replay and saved projects.
They are software models; retired-instruction ticks are not measured hardware time.

## Text and monochrome display

Open **View → Graphical display** (Ctrl+Alt+D), or create one of the two
display templates. The examples `demos/c/text_display.c` and
`demos/c/virtual_display.c` are also available. The Display pane shows the image,
a selectable text/pixel description, and **Save display PNG** / **Save frame JSON**.
The JSON contains dimensions, mode, buffer address, exact bytes and their SHA-256.
Each comparison machine has its own Display tab. Add `$display` to comparison
observations to compare mode, dimensions and exact frame bytes independently of
where each compiler placed the buffer. Comparison reports include both frames.

Include `<rv_display.h>` and allocate a static `volatile uint8_t` buffer. Call
`rv_display_configure(buffer, width, height, mode)`; zero means success and -1
means invalid mode, dimensions or buffer range. A rejected request preserves the
previous configuration. `rv_display_disable()` switches it off.

- `RV_DISPLAY_TEXT`: 1–80 columns × 1–25 rows, one Latin-1 byte per cell. Control
  bytes appear as spaces. `rv_display_text` writes from a given column/row to the
  end of that row. Unicode byte sequences are not combined into characters.
- `RV_DISPLAY_BITMAP`: 1–128 columns × 1–64 rows, monochrome. Each row consumes
  `(width + 7) / 8` bytes. Bit 0 is the leftmost pixel in each byte; any unused
  final bits remain padding. `rv_display_pixel` sets or clears one bounded pixel.

The helpers check coordinates and dimensions. The caller owns the buffer and must
allocate the full size. Stores update ordinary simulated RAM, so Back/Replay can
show individual drawing changes without copying a separate framebuffer into each
history event. Reset disables the display and restores the loaded RAM image.
The selected main Display/Console tab is saved with the workspace.

The teaching ECALL ABI uses `a7=0x120`, `a0=buffer address`, `a1=width`, `a2=height`
and `a3=mode`; the result is `a0=0` or `UINT32_MAX`. Disabling requires zero in all
four arguments. Like the other Studio serviced ECALLs, the operation advances PC
and records a reversible event; the trapping ECALL is not counted as a retired
CPU instruction. This ABI is separate from Linux system calls and hardware MMIO.

The batch `--json` report includes numeric display mode/address/dimensions and
`dataHex`. It is useful for checking a frame without launching the desktop.

## UART serial framing

Open the **UART** tab in the same device panel. The UART loopback template sends
the value 42 through the simulated TX line and receives it on RX. The examples
`demos/c/uart_loopback.c` and `demos/c/uart_receive.c` demonstrate a whole string
and external input. Each comparison machine also has a read-only UART panel.

`<rv_uart.h>` exposes a deterministic 8-bit serial model. Configure
`rv_uart_configure(ticks_per_bit, parity, stop_bits, loopback)`, with 2–65535
retired instructions per bit, `RV_UART_NONE`, `RV_UART_EVEN` or `RV_UART_ODD`,
and 1 or 2 stop bits. Success returns zero and clears queues/counters; an invalid
request returns -1 without changing the previous configuration. Loopback connects
the TX waveform to RX. `rv_uart_disable()` and machine Reset disable the device.

Transmission sends a low start bit, eight data bits with bit 0 first, optional
parity and high stop bits. RX checks the start bit and samples at bit centers.
Wrong parity or a low stop bit discards that frame and increments the corresponding
error counter. A full receive FIFO drops the newest valid byte and increments
the overrun counter. Back-to-back valid frames are supported. After a malformed
stop, the receiver needs a new falling edge to recognize another frame, as with
a serial line; following corrupted input can therefore take time to resynchronize.

- `rv_uart_putc(byte)` is nonblocking: zero queues a byte; -1 means TX is full or
  disabled. `rv_uart_getc()` returns 0–255, or -1 for an empty/disabled RX FIFO.
- `rv_uart_status()` returns `RV_UART_ENABLED`, `RV_UART_TX_READY`,
  `RV_UART_RX_READY`, `RV_UART_TX_BUSY`, `RV_UART_INPUT_BUSY` and `RV_UART_ERROR`
  flags. TX busy includes its shift register; a received byte can be available
  before the sender finishes the last stop bit.
- `rv_uart_clear_errors()` clears parity/framing/overrun counters.
- TX, RX and external input queues hold 16 frames each. A shifting frame is
  separate from its waiting queue. Reset discards UART input; console input keeps
  its existing separate reset behavior.

For external RX, pause a configured program with loopback off, enter UTF-8 text
or complete hexadecimal byte pairs, and choose **Queue RX frames**. Input is
recorded in history; Run shifts its bits. Optional controls deliberately inject
wrong parity or missing stop bits. Parity injection requires parity to be enabled.
The panel reports lines, pending bytes, completed frames and errors, with readable
text that does not depend on colors. The guest consumes RX bytes normally.

**Save UART state** exports the full portable JSON snapshot. `$uart` comparison
observations compare configuration, active queued bytes, shift phases, last bytes
and counters; unused FIFO slots and inactive decoder scratch are ignored. This
checks the current state, not an unbounded transcript of previous transmitted
bytes. The JSON comparison report preserves the complete model state.

**Export UART VCD** includes TX/RX; **Export UART to Sinalis** exports one selected
line. Both use an explicit user-selected duration per retired-instruction tick.
Manual input edits and serviced teaching ECALLs do not advance the UART clock;
same-time samples are coalesced in Sinalis JSON. Exports include only applied,
retained history and report how many earlier events were dropped. The instruction
clock is reproducible for a fixed program/input; different compiler optimizations
can change its timing. This is a teaching model without a baud-rate or CPU-cycle
accuracy claim, host serial ports, analog noise or clock-drift modeling.

The teaching ECALLs are `0x130` configure (`a0=ticks`, `a1=parity`, `a2=stops`,
`a3=loopback`), `0x131` put (`a0=byte`), `0x132` get, `0x133` status and `0x134`
clear errors. Results are in `a0`, with `UINT32_MAX` representing -1. The batch
JSON report includes UART configuration, lines, frame/error counters and RX bytes.

## PWM, ADC, SPI e I²C

Os quatro periféricos adicionais, painel, SDK, exemplo, mapa esparso MMIO,
reversão e limites estão em [PERIPHERALS.md](PERIPHERALS.md).
