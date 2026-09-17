# Third-party notices

RISC-V Studio uses independent components with their own licenses. The
application license does not restrict rights those licenses grant. The listed
components are not represented as original work by the application author.

## Components

| Component | Distributed use | License information |
| --- | --- | --- |
| Qt 6.10.2 on Windows | Core, Gui, Widgets, Concurrent and required platform/image/style plugins, loaded as shared libraries. | LGPL-3.0; full LGPL/GPL texts and bundled-component notices in licenses/qtbase and licenses/. |
| Qt on Linux | Supplied by the system package manager; Qt 6.4+ runtime. | System package copyright files and LGPL/GPL terms. |
| xPack RISC-V GCC 15.2.0-1 | Windows C compiler, assembler/linker, binutils, target headers and selected RV32I libraries. | GCC/binutils GPL terms; GCC Runtime Library Exception for covered runtime code; component licenses in toolchain/distro-info/licenses/. |
| GNU Windows runtime 13.2.0 / MinGW-w64 | libgcc, libstdc++ and POSIX thread runtime used by the Windows application. | GCC GPL with applicable Runtime Library Exception; MinGW-w64 and winpthreads notices in licenses/. |
| CPython 3.13.7 | Windows embedded Python runtime and standard library. | Python Software Foundation license and included third-party terms: licenses/Python-LICENSE.txt. |
| pyelftools 0.33 | Windows ELF/DWARF metadata reader. | BSD-style license: licenses/pyelftools-LICENSE.txt. |
| zlib 1.3.1 | Project compression and the Windows portable launcher. | zlib license: licenses/zlib-LICENSE.txt. |
| elfutils libdw/libelf | Linux ELF/DWARF reading through system shared libraries. | System package licenses; library LGPL terms. |
| NSIS 3.10 | Windows installation/uninstallation stub. | NSIS and included component terms: licenses/NSIS-copyright.txt. |

The cross-compiler distribution also carries notices for its dependencies,
including GMP, MPFR, MPC, ISL, zlib, zstd, libiconv and other upstream components.
Their own license files identify the relevant authors, terms and exceptions.
The Qt source notice directory includes third-party licenses used by Qt.
The Qt SDK inventory can mention tools/modules beyond the subset shipped in the
application; those build tools are not added to the public application folder.

## Source availability

[Download the complete dependency source bundle](https://github.com/IcaroRamosDM/riscv32-studio/releases/download/v1.0.0/RISC-V-Studio-1.0.0-Third-Party-Sources.zip).
The bundle expands into the `third-party-sources/` directory described below.

The release accompanies the binaries with a separate **third-party-sources**
directory containing upstream source archives, the xPack build recipe and helper
patches, and the Ubuntu MinGW/GCC source-package inputs. SOURCE_INDEX.json records
download locations and SHA-256 hashes. Qt source corresponds to 6.10.2.
These archives are development/license materials and are not needed to run Studio.

When distributing the release, keep the dependency-source download available
alongside the binary downloads and retain this notice and the component licenses.
Linux system dependencies and their sources are supplied by the distribution's
package repositories.

Primary upstream locations:

- [Qt 6.10.2 sources](https://download.qt.io/archive/qt/6.10/6.10.2/submodules/)
- [Qt LGPL obligations](https://www.qt.io/development/open-source-lgpl-obligations)
- [xPack RISC-V GCC 15.2.0-1](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/tree/v15.2.0-1)
- [xPack build helper 4.3.15](https://github.com/xpack-dev-tools/xbb-helper-xpack/tree/v4.3.15)
- [GCC source releases](https://ftp.gnu.org/gnu/gcc/)
- [GCC Runtime Library Exception](https://www.gnu.org/licenses/gcc-exception-3.1.html)
- [MinGW-w64](https://www.mingw-w64.org/)
- [Python 3.13.7](https://www.python.org/downloads/release/python-3137/)
- [pyelftools](https://github.com/eliben/pyelftools)
- [zlib](https://zlib.net/)
- [elfutils](https://sourceware.org/elfutils/)
- [NSIS license](https://nsis.sourceforge.io/License)

## Replacing compatible shared libraries

The Windows installer and portable ZIP retain Qt and runtime DLLs as separate
files. An interface-compatible modified library can replace the corresponding
file in that application folder. Keep the original files as a backup. For the
single EXE, use the matching portable ZIP to obtain the same inner application
in an ordinary folder before replacing libraries.

Linux uses ordinary shared-library linking and the installed distribution
libraries. Follow the distribution's library search/loading rules for replacements.
No application license term forbids the reverse engineering needed to debug
modifications to LGPL-covered libraries.

Qt, GCC, RISC-V and other names identify their respective projects. Their inclusion
does not imply those projects endorse or certify RISC-V Studio.
