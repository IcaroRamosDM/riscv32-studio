# Portable Linux 1.9.0-rc1

Download the portable TAR from the
[official prerelease](https://github.com/IcaroRamosDM/riscv32-studio/releases/tag/v1.9.0-rc1).
It targets **Ubuntu 26.04 x86-64 / glibc >= 2.43** with a graphical desktop.
The launcher checks glibc before starting. Debian 12/13 and Ubuntu 24.04 are
outside this package's supported baseline.

## Extract and open

Extract the entire `riscv32-studio` folder and keep its contents together.
Run `./RISC-V-Studio` inside it. `./riscv32-studio` opens the classic CLI.
The archive preserves executable permissions; no manual chmod is required.
Save programs and projects in your own writable folder outside the application.

The package includes Qt, Python, RISC-V compilers, GDB, the architectural
worker, guest libraries, fonts, examples and offline help. Native host C export
requires a separately installed C compiler and Make.

Run `./install-desktop.sh` to register an application shortcut and project
association for your user. Run it again after moving the folder.
`./uninstall-desktop.sh` removes that registration and preserves the application
and user projects.

## Verify integrity

Download SHA256SUMS.txt from the same release. Compare the archive's local
`sha256sum` result with its entry before extracting. From the extracted folder,
verify every recorded application file and permission:

```sh
python/bin/python3 tools/verify_portable.py .
```

The Python wrapper uses its own standard library; ELF runtime paths resolve
bundled libraries. The manifest identifies the exact delivered files and their
origins. Hashes establish consistency with the release's records; they are not
a publisher signature.

## First run

Open About and confirm **1.9.0-rc1**. The advanced C17/Assembly workspace opens
by default. Start a tutorial and use Terminal for input/output.
