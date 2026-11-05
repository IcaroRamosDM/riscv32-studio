# From your first program to RISC-V debugging

The advanced workspace opens by default. **Learn by doing**, above the editor,
opens **Tutorials and examples**; the same command is in **Help**.
Browsing shows a guide and a source preview. **Open editable example** creates
a draft and selects its ISA, environment and hart count, after confirming
any unsaved sources.

## Print a message

[Start with Hello World](tutorial:hello). Choose **Build & load**
(**Ctrl+Enter**), then **Run** (**F5**). Read the **Terminal** panel at the
bottom of the workspace. Building prepares a program; running produces its
output. **Build log** contains compiler messages. `printf`, `puts` and
`putchar` write to Terminal. The original example exits with status **0**.

If Terminal is closed, use **View > Panels > Terminal**, or **Open Terminal**
in the guide. Edit the greeting, rebuild and run the new version.

## Read input with scanf

[Open the input tutorial](tutorial:input). Run, type `Ada` in the field below
Terminal output and press **Enter**. Use one name without spaces: `%31s`
reads one word in this example. At the second prompt, send `7` and Enter.
Execution resumes when input arrives. The final message is:

```
Hello, Ada! Next year: 8.
```

The example bounds the `scanf` fields and validates the number before
converting it. Reset and try `abc`, `-1` or `1000` as the number to see its
error path. **Send EOF** ends the stream: the example exits with status **1**
if the name is missing, or **2** if the number is missing. Reset reopens input.
Terminal is the simulated program's standard I/O; UART is a separate serial
device with its own queues and transmission errors.

## Follow and reverse a change

[Open the reversible example](tutorial:reverse), build at optimization **0**
and double-click `result` in **Symbols** to inspect its address in **Memory**.
**Step instruction** (**F6**) follows the stores. **Back** (**F7**) undoes a
recorded instruction; **Replay forward** (**F8**) reapplies it. Running to the
end prints `result = 42`.

Double-click an Assembly instruction to toggle a breakpoint; Run stops before
that instruction. Start the C debugger to inspect calls and locals: **F11**
steps into a function, **F10** steps over it and **Shift+F11** returns.
Editing the source leaves the already loaded instructions in place. The
Source title shows when they differ; rebuild and load to execute the edit.
The tutorial confirms its reference outcome only for the matching loaded source.

## Write Assembly directly

[The Assembly example](tutorial:assembly) supplies `_start` and uses `lbu`,
`addi`, a branch and `ecall`. Inspect `t0`, `t1`, `a0` and `a7`. In this RV64I
example, service **0x100** prints the byte in `a0`; service **93** exits with
its status in `a0`. Put the service number in `a7`.

These are Studio **Teaching** services. Linux system calls such as `write`
are not automatically services of this environment. Firmware can instead
install its own trap handlers and use memory-mapped peripherals.
Compare `la` and `li` with their expanded instructions. Edit the message;
the label difference supplies its byte length.

[Combine C and inline Assembly](tutorial:mixed) next. Operand constraints
explain inputs and outputs to the compiler. To use a separate `.S` source,
add it to the project and mark it for compilation. Select an ISA that contains
the instructions used by the program.

## Explore a feature through its result

- [Virtual files](tutorial:files) creates `result.txt` in simulated RAM.
  **View > Panels > Program files** shows its bytes. **Save file** exports a
  copy to a location you choose on the notebook.
- [Graphical output](tutorial:display) draws 64 pixels on a 64 by 32 display.
  **Open inspection panel** reveals it. Step through the loop and reverse
  the pixel stores.
- [C17 threads](tutorial:threads) uses two harts, `threads.h` and
  `stdatomic.h`. **C17 hosted threads** starts `main` once; joining the
  thread leaves the shared counter at **200**.
- [LR/SC](tutorial:atomic) shows a reservation and conditional store.
  Editing the reserved bytes between LR and SC cancels the reservation.
- [Floating point](tutorial:floating) compares **5.25** with its binary64
  bits and rounding flags.
- [Vectors](tutorial:vector) loads four elements and produces
  `[11, 12, 13, 14]` in vector registers and RAM.
- [Timer interrupts](tutorial:timer) uses the **Firmware** environment.
  Inspect `mepc`, `mcause`, `mstatus` and the three interrupts that print `123`.

Hart interleaving is deterministic and replayable; one run does not explore
every physical weak-memory outcome. The timer follows simulated instruction
progress, rather than physical circuit timing.

## Arrange and save the workspace

Each panel has compact title-bar controls to close, move or detach it.
**View > Panels** reopens it. **Restore default layout** recovers the original
arrangement; **Undo layout reset** recovers your previous arrangement.

Open **View > Panels > History** to inspect recent records. **Older** and
**Newer** browse the list; **Latest** follows new activity. Each range contains
up to 256 records, and one row can group several instructions. The counter shows
the visible range and the machine's history position. Use **Back** (**F7**) and
**Replay forward** (**F8**) to restore states. Executing or editing from an
earlier position replaces its previously recorded future.

**Save source** (**Ctrl+S**) saves editor text. **Save workspace project**
(**Ctrl+Shift+S**) keeps sources, settings, the loaded program and retained
history in a `.rvstudio` file. Reopen it to continue inspection. An exported
file on the notebook is independent; reversing the simulator does not rewrite
that exported copy.

**Next example** only browses. Follow **Try it**, establish the original
result and then change one thing under **Explore further**. The guide never
replaces your source or executes it automatically.
