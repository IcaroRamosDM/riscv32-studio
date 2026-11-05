# Dependency source availability — 1.9.0-rc1

The Linux prerelease provides `RISC-V-Studio-1.9.0-rc1-Third-Party-Sources.tar.gz` alongside its binary downloads.
The independent components retain their licenses; the Studio application
license does not restrict their rights.

The archive contains:

- `third_party/`: pinned upstream GCC 16.2, LLVM/Clang 21.1.8, binutils 2.47,
  GDB 17.1, the architectural model and guest-library sources, with their
  patches, copyright notices and source metadata.
- `tools/`: the dependency preparation/build helpers distributed with Studio.
- `toolchain/provenance/`: the recorded toolchain configurations and manifests.
- `native/`: exact Ubuntu source packages for the redistributed native
  libraries, Qt, Python, language tools and fonts, including upstream tarballs,
  Debian/Ubuntu patches and `.dsc` build metadata.
- `NATIVE-SOURCES.json` and `SOURCE-FILES.json`: component origins, source
  descriptor SHA-256 records and an inventory of the delivered source files.

For a native component, enter its directory and use `dpkg-source -x file.dsc`
to unpack the upstream source and distribution patches. Its `debian/` files
contain the distribution build rules and requirements. The downloaded source
payloads are checked against the SHA-256 records in the corresponding `.dsc`.
Each component's license/copyright files remain in its source package.

Compiler/model sources and preparation helpers are also retained inside the
portable application under `third_party/` and `tools/`. Toolchain manifests
identify original and packaged files. Historical paths in build provenance
identify the original build environment and are not required install paths.

Qt and the native libraries remain separate shared objects in the Linux
package. Compatible modified versions can replace the corresponding library
files. The application license does not forbid reverse engineering needed
to debug modifications to LGPL-covered libraries. A full integrity check
reports intentional file replacements as differences from the original package.

[Official candidate release](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.9.0-rc1). The
[1.0.0 release](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.0.0) keeps its separate original source download.
GitHub's automatic Source code ZIP/TAR is the public documentation repository,
not this matching dependency-source archive or the private desktop source.
