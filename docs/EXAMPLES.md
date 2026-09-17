# Guided examples

Open **Examples** and select the named file. Choose **Learning (-O0)**, click
**Build**, then **Load build**. The machine starts paused. Use **Step instruction**
(F6), **Into C**, or **Run** (F5). Choose **Unlimited** speed for complete runs.
Every C example returns exit code 0 when its own checks pass.

Open **Inspect > Symbols**, find the named global, and double-click it to inspect
its address in **Memory**. Each byte is hexadecimal; a 32-bit integer uses four
bytes, least significant byte first. Addresses may change between compilations.

| Example | What to observe | Expected after Run |
| --- | --- | --- |
| learning.c | Two calls, arrays and source-to-instruction mapping | result=72; calls=2 |
| arithmetic.c | Software multiplication/division on RV32I | product=-456765; quotient=-333; remainder=-24 |
| decisions.c | Signed versus unsigned comparison of the same bits | signed_less=1; unsigned_less=0; result=100 |
| loops.c | Conditional branch returns to an earlier instruction | result=55 |
| functions.c | Calls and recursion; compare optimized control flow | result=120 |
| arrays.c | Consecutive words change in RAM | values={4,7,10,13,16}; result=50 |
| pointers.c | Word pointer followed by byte access | values[1]=298; bytes 2A 01 00 00 |
| stack.c | sp decreases during recursive calls | result=10; lowest_sp is below entry_sp |
| optimization.c | Source operations can disappear or merge | Both builds exit 0; inspect instruction counts |
| stress.c | Larger array and bounded execution history | result=8386560; values[4095]=4095 |

## Decisions: bits acquire meaning at an operation

Run decisions.c. The input -3 has the 32-bit pattern 0xFFFFFFFD.
Its unsigned interpretation is 4294967293. The signed comparison is true; the
unsigned comparison is false. Locate the comparisons in Assembly and read
Instruction details. The compiler may use a comparison instruction or a branch.

## Calls and stack

Load functions.c at -O0. Put a source breakpoint inside factorial by clicking its
line-number gutter. Run, then use Into C and Over. Inspect Variables / stack,
ra (return address), and sp (stack pointer). Out runs until the tracked call
returns. Back restores the preceding recorded CPU and memory state; Replay
restores the next recorded state.

For measurable stack movement use stack.c. Its noinline attribute and volatile
locals intentionally preserve recursive frames in both compilation profiles.
The inline Assembly only observes sp; it does not move the stack.

## RAM capacity and a deliberate access error

Load ram_boundary.asm. With 64KiB RAM, access to 0x00010000 is outside RAM.
Build, Load build, Run: expect an out-of-bounds store.
Change RAM to 128KiB, Build and Load build again, then Run. The same address is
now installed RAM; inspect it in Memory. Increasing RAM does not create all
2^32 possible byte addresses. Changing the setting takes effect on build/load.

## Optimization

Open optimization.c and click Compare -O0 / -O2. Run both and compare exit codes,
data results and retired instruction counts. Compare C line inspects a source
location without executing either CPU. Type a line or use its arrow buttons.
A line without a mapping is explicitly reported; it can be eliminated, merged,
reordered, or absent from the available debug data.

## Bounded history and responsiveness

Open stress.c. Set Speed to Unlimited, Run limit to 2000000, and History to
100000. Build, Load build, Run. Older entries are discarded when the configured
capacity is reached. History is a ring buffer, not a recording of unlimited
length. Use a small value such as 64 to see discarding sooner; 0 disables history.
Pause remains available during execution. At -O2 there may be fewer than 100000
instructions; compare -O0 to exercise the full buffer.
