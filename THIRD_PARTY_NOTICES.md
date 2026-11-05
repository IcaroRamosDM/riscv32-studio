# Third-party notices

RISC-V Studio 1.9.0-rc1 bundles independently licensed Qt 6.4.2,
CPython 3.14, GCC 16.2, LLVM/Clang 21.1.8, GNU binutils 2.47, GDB 17.1,
the pinned architectural model, guest libraries and native dependencies.
Their inclusion does not imply upstream endorsement or certification.

The application retains each component's notices under `licenses/`,
`licenses/native-linux/`, `third_party/` and the toolchain directories.
`PORTABLE_MANIFEST.json` identifies the files and their original origins;
toolchain provenance records the corresponding build configuration and patches.

- Qt libraries retain LGPL/GPL terms and bundled third-party notices.
- GCC, binutils and GDB retain their GPL terms; covered GCC runtime code
  additionally carries the GCC Runtime Library Exception.
- LLVM/Clang and compiler-rt retain Apache-2.0 WITH LLVM-exception terms.
- The architectural model, Picolibc, musl adaptations, SoftFloat and pyelftools
  retain their BSD-style/MIT and other supplied component terms.
- CPython retains the PSF license and its included third-party licenses.
- Native libraries and fonts retain their individual distribution copyright
  and license records. They are not all covered by one common license.

[Matching dependency sources](https://github.com/IcaroRamosDM/riscv32-studio/releases/download/v1.9.0-rc1/RISC-V-Studio-1.9.0-rc1-Third-Party-Sources.tar.gz) accompany this
release. See [source availability and replacement information](SOURCE_AVAILABILITY.md).
Source payloads are checked against source-package descriptors and inventories.
Studio's closed-source application license does not limit rights granted by
any component license.

Historical 1.0.0 components and their original source download remain in
[the 1.0.0 release](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.0.0) and its
[versioned notices](https://github.com/IcaroRamosDM/riscv32-studio/blob/v1.0.0/THIRD_PARTY_NOTICES.md).
