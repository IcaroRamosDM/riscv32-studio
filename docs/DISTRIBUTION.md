# RISC-V Studio downloads

## Linux 1.9.0-rc1

This prerelease targets **Ubuntu 26.04 x86-64 / glibc >= 2.43**. It includes
Qt 6.4.2, Python, the architectural worker, GCC 16.2, Clang 21.1.8, clangd,
GNU binutils, GDB 17.1, guest libraries, examples and offline help.

| File | Purpose |
| --- | --- |
| RISC-V-Studio-1.9.0-rc1-Linux-amd64.deb | Ubuntu installer, desktop integration and application under `/opt/riscv32-studio`. |
| RISC-V-Studio-1.9.0-rc1-Linux-x86_64-portable.tar.gz | Complete portable folder; extract it and run `RISC-V-Studio`. |
| SHA256SUMS.txt | SHA-256 checksums for this release's downloads. |
| RISC-V-Studio-1.9.0-rc1-Third-Party-Sources.tar.gz | Matching dependency sources, patches, build helpers and source indexes. |
| LEIA-ME.txt / NOTAS_DA_VERSAO.md | Portuguese installation instructions and release notes. |
| LICENSE.txt / SOURCE_AVAILABILITY.md | Application license and independent dependency source information. |

[Open the release](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.9.0-rc1).
See [installation](../HOW_TO_USE.md#1-install-and-open) and
[portable instructions](PORTABLE_LINUX.md).

A graphical desktop is required. Debian 12/13 and Ubuntu 24.04 do not meet this
package's glibc requirement. Building exported native host C
programs requires a separately installed C compiler and Make.

## Previous release

The [1.0.0 release](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.0.0)
remains available with its original Windows/Linux packages, checksums, dependency
source ZIP and documentation. Each tag retains its own documentation and downloads.
The latest stable release remains 1.0.0 while 1.9.0-rc1 is a prerelease.

GitHub's automatic Source code ZIP/TAR contains the public documentation and
examples. It is not an application installer or the matching dependency-source
bundle.
