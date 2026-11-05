<p align="center"><img src="assets/app-icon.png" width="96" alt="RISC-V Studio icon"></p>

[![Distribution checks](https://github.com/IcaroRamosDM/riscv32-studio/actions/workflows/checks.yml/badge.svg)](https://github.com/IcaroRamosDM/riscv32-studio/actions/workflows/checks.yml)

# RISC-V Studio

Write C or Assembly and inspect a RISC-V program instruction by instruction.
Follow source mappings, registers, memory, variables and execution history.
The current Linux candidate adds an advanced RV32/RV64 workspace, interactive
Terminal input/output, detachable panels and integrated language tools.

**Free for personal and educational use.** Commercial use outside the
educational permission requires the author's prior written authorization.
[Application license](LICENSE.md).

This repository provides official downloads, documentation and examples.
GitHub’s **Source code** archives contain the documentation and examples. Download a package below to install Studio.

## Download

| Version | Platform and status | Downloads |
| --- | --- | --- |
| **1.9.0-rc1** | **Linux prerelease**, Ubuntu 26.04 x86-64 / glibc >= 2.43 | [DEB installer](https://github.com/IcaroRamosDM/riscv32-studio/releases/download/v1.9.0-rc1/RISC-V-Studio-1.9.0-rc1-Linux-amd64.deb) · [Portable TAR](https://github.com/IcaroRamosDM/riscv32-studio/releases/download/v1.9.0-rc1/RISC-V-Studio-1.9.0-rc1-Linux-x86_64-portable.tar.gz) |
| **1.0.0** | Previous stable release, Windows and Linux | [Keep using or download 1.0.0](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.0.0) |

[Latest candidate and release notes](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.9.0-rc1) ·
[All releases](https://github.com/IcaroRamosDM/riscv32-studio/releases) ·
[Candidate SHA-256 checksums](https://github.com/IcaroRamosDM/riscv32-studio/releases/download/v1.9.0-rc1/SHA256SUMS.txt) ·
[Instruções em português](docs/releases/1.9.0-rc1.md)

The Linux candidate includes Qt, Python, RISC-V compilers, GDB, guest
libraries, examples and offline help. It requires a graphical desktop.
Debian 12/13 and Ubuntu 24.04 do **not** meet this package's glibc requirement.
For Windows, use the installer or portable package from version 1.0.0.

## Install the Linux candidate

For the DEB, open it with Ubuntu's package installer or run:

```sh
sudo apt install ./RISC-V-Studio-1.9.0-rc1-Linux-amd64.deb
```

Open **RISC-V Studio** from application search. Its installed executable is
`/opt/riscv32-studio/RISC-V-Studio`.

For the portable archive, extract the whole folder and run **RISC-V-Studio**
inside it. `install-desktop.sh` registers an application shortcut;
`uninstall-desktop.sh` removes that registration. Register again after moving
the folder. Save your source files and projects outside the application folder.

Before installation, compare your package's `sha256sum` result with its entry
in **SHA256SUMS.txt** from this release. After portable extraction, check the
complete application from its folder with:

```sh
python/bin/python3 tools/verify_portable.py .
```

## First program

Open a C input/output tutorial, build it and run it. Use **Terminal** to see
`printf`/`puts` output and type the values requested by `scanf`. An Assembly
Hello World tutorial is also included. **View** (**Exibir** in Portuguese)
reopens panels and restores the default layout; panels can be detached.

## Current features

- Classic teaching workspace and advanced RV32/RV64 architectural profiles.
- C17 with GCC 16.2, GNU Assembly, Clang 21.1.8, clangd and GDB 17.1.
- Standard input/output, virtual files, threads and thread-local storage,
  plus optional Annex K interfaces in the derived guest SDKs.
- Breakpoints, source and instruction stepping, register/memory inspection,
  variable inspection, reversible history and saved projects.
- Detachable panels, visibility controls and default-layout restoration.
- Readable examples, tutorials and offline help; four-space editor indentation.

Available extensions, devices and debugger behavior depend on the selected
workspace and profile. See [model limits](docs/ISA_MODEL_LIMITS.md).
Exporting native host C programs requires a separate system C
compiler and Make.

## Guides

- [Practical guide in Portuguese](docs/GUIA_PRATICO.md)
- [How to use](HOW_TO_USE.md)
- [How RISC-V works](HOW_RISCV_WORKS.md)
- [Advanced workspace](docs/ADVANCED_GUIDE.md)
- [Terminal and panels](docs/TERMINAL_AND_WORKSPACE.md)
- [C standard library](docs/C_STANDARD_LIBRARY.md)
- [Linux release notes](docs/releases/1.9.0-rc1.md)

The 1.0.0 release retains its
[original documentation](https://github.com/IcaroRamosDM/riscv32-studio/tree/v1.0.0).

## Dependency sources and licenses

[Download the matching dependency sources](https://github.com/IcaroRamosDM/riscv32-studio/releases/download/v1.9.0-rc1/RISC-V-Studio-1.9.0-rc1-Third-Party-Sources.tar.gz).
These are not needed to run Studio. They include pinned upstream archives,
local patches/build helpers and the exact Ubuntu native source packages.
[Source availability](SOURCE_AVAILABILITY.md) and
[third-party notices](THIRD_PARTY_NOTICES.md) explain their contents.
The application's license does not limit independent dependency rights.

## Report a problem

[Open an issue](https://github.com/IcaroRamosDM/riscv32-studio/issues) with the
application version, operating system, selected workspace/profile, reproduction
steps and expected/observed behavior. Attach only files you may share publicly.

## Author

**Icaro Ramos Rodrigues dos Santos**<br>
Electronic Engineer

Embedded Systems | Firmware | C | C++ | Python | Dart | Assembly | Hardware Design | PCB Design | STM32 | ARM Cortex-M | Linux | Windows | RISC-V | Verilog | SystemVerilog | UVM | Design Verification | Instrumentation | Signal Processing | Qt | FLTK

GitHub: [IcaroRamosDM](https://github.com/IcaroRamosDM)<br>
LinkedIn: [icaro-ramos-r](https://www.linkedin.com/in/icaro-ramos-r/)<br>
[icaroelt@gmail.com](mailto:icaroelt@gmail.com)
