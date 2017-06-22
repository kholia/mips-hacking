Building Linux kernel for MIPS
------------------------------

* Get "mips32 musl" cross-compilation toolchain from http://toolchains.free-electrons.com/.

  ```
  $ cd ~

  $ tar -xjvf mips32--musl--bleeding-edge-2017.05-toolchains-1-2.tar.bz2
  ```

* Build Linux kernel with this toolchain.

  ```
  $ cd <kernel-source-tree>
  ```

  ```
  $ make ARCH=mips CROSS_COMPILE=$HOME/mips32--musl--bleeding-edge/bin/mips-linux- menuconfig
  ```

  * Under Machine selection, Select System type (MIPS Malta board)

  * Under Linux Kernel Configuration, Select Endianess selection (Big endian)

  ```
  $ make ARCH=mips CROSS_COMPILE=$HOME/mips32--musl--bleeding-edge/bin/mips-linux- -j8
  ```

* Use the generated `vmlinux` file for booting a MIPS system.
