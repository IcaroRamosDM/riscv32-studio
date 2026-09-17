# RISC-V Studio 1.0.0 distributions

## Release files

| File | Contents |
| --- | --- |
| RISC-V-Studio-1.0.0-Windows-Setup.exe | Per-user installer, Start menu entry, optional desktop shortcut, project association and uninstaller. |
| RISC-V-Studio-1.0.0-Windows-Portable.zip | Complete Windows folder. Extract it before running the inner RISC-V Studio.exe. |
| RISC-V-Studio-1.0.0-Windows-Portable.exe | Single compressed executable that prepares a temporary workspace and runs the same application. |
| riscv32-studio_1.0.0_amd64.deb | Native Debian/Ubuntu package with menu/icon/project integration and declared runtime dependencies. |
| riscv32-studio-1.0.0-linux-x86_64.tar.gz | Native Linux folder with dependency and per-user desktop-registration scripts. |
| third-party-sources/ | Corresponding dependency source archives, build recipes, patches and their hash/index file. |
| SHA256SUMS | Checksums of the deliverable files. |

The three Windows editions have the same simulator features. Installer and ZIP
include the RISC-V compiler and Python. The single EXE compresses those files;
it still needs disk space for the extracted workspace during execution.

The Linux archive uses installed system dependencies. It is relocatable after
extraction; its per-user menu entry records the folder location and should be
registered again after moving that folder.

## Platform baseline

Host architecture: **x86-64**. Guest architecture: **RV32I / ILP32**, little endian.

Linux binaries are built against Debian 12 with GCC 12 and Qt 6.4.2. The package
requires glibc 2.36 or newer and compatible Qt 6 libraries. Compatibility checks
target Debian 12, Debian 13, Ubuntu 24.04 and Ubuntu 26.04. 

Windows binaries use Qt 6.10.2 and the POSIX MinGW runtime. They target Windows
10/11 x64.

ARM hosts, 32-bit hosts, macOS and Linux distributions without compatible runtime
packages are not covered by these binaries.

## Installation

Use the exact steps in [How to use](../HOW_TO_USE.md#1-install-and-open).

The Windows installer defaults to the current user's LocalAppData/Programs folder.
The Linux DEB installs application files in /opt/riscv32-studio and registers
the desktop entry, icons and .rv32project MIME type in standard system locations.

Compilation and comparison use private system temporary directories, so normal
users do not need write access to the Linux installation directory.

## What is preserved

Save sources and projects in a chosen user folder. Project opening restores a
paused session. Application installation is separate from user documents.

The Windows uninstaller deletes its listed application files and empty application
subdirectories; it does not recursively delete arbitrary files in the installation
folder. Linux removal uses the package manager's file list. Portable desktop
removal removes its managed registration; it does not delete the application.

The single EXE removes unchanged files it extracted after the child application
exits. Keep long-lived projects outside its temporary workspace.

## Licenses and sources

The [application license](../LICENSE.md) defines personal, educational and
authorized commercial use.
[Third-party notices](../THIRD_PARTY_NOTICES.md) apply independently to Qt, GCC,
Python and other dependencies. The dependency-source files accompany the release
separately and do not enlarge the executable's compressed payload.

## Verify a download

Linux, in the release directory:

~~~bash
sha256sum -c SHA256SUMS
~~~

Windows PowerShell, for one selected file:

~~~powershell
Get-FileHash -Algorithm SHA256 -LiteralPath '.\RISC-V-Studio-1.0.0-Windows-Setup.exe'
~~~

Compare the resulting hash with that filename's entry in SHA256SUMS. A matching
hash establishes file consistency with the provided list; it is not a publisher
signature.
