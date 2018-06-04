#### Easily start a mips virtualmachine with arm_now

```sh
$ sudo pip3 install https://github.com/nongiach/arm_now/archive/master.zip
$ arm_now start mips32el
Welcome to Buildroot
buildroot login: root
# uname -m
mips
# gdb /bin/ls
(gdb) start
Temporary breakpoint 1, 0x00405434 in main ()
(gdb) x/i $pc
=> 0x405434 <main+12>:	li	a0,-1
```

#### Get Debian for MIPS

* http://ftp.debian.org/debian/dists/stretch/main/installer-mips/current/images/malta/netboot/

* Download both files.

#### Building QEMU (optional)

```
./configure --prefix=$HOME/QEMU --target-list=mips-softmmu,mips-linux-user

make

make install
```

#### HOWTO

##### Install

```
qemu-img create -f qcow2 hda.img 32G

qemu-system-mips -M malta -m 256 -hda hda.img -kernel vmlinux-4.9.0-3-4kc-malta \
    -initrd initrd.gz -append "console=ttyS0 nokaslr" -nographic
```

Copy `initrd.img-4.9.0-3-4kc-malta` from the installed VM to the host machine.

##### Boot:

```
qemu-system-mips -M malta -m 256 -hda hda.img -kernel vmlinux-4.9.0-3-4kc-malta \
    -initrd initrd.img-4.9.0-3-4kc-malta \
    -append "root=/dev/sda1 console=ttyS0 nokaslr" -nographic \
    -netdev user,id=net0 \
    -device e1000-82545em,netdev=net0,id=net0,mac=52:54:00:c9:18:27 \
    -net user -redir tcp:2222::22
```

`C-a h` key combination is useful to interacting with QEMU in `-nographic` mode.

#### Notes

* Kernel (and initrd.gz) from Debian 9 MIPS (version 20170615) does not boot in
  QEMU 2.9.0. It fails with `Initramfs unpacking failed: uncompression error` or
  `Initramfs unpacking failed: junk in compressed archive` error messages.

  Update: In Debian 9, the initrd load address clashes with kernel address
  randomization due to a bug in QEMU. Pass "nokaslr" to the append option.

* Debugging a MIPS binary directly under QEMU,

  ```
  $ qemu-mips -g 1234 ./mips.binary

  $ gdb ./mips.binary  # in another terminal
  (gdb) set architecture mips
  (gdb) target remote localhost:1234
  ```

  Run dynamically linked MIPS binary with QEMU,

  ```
  LD_LIBRARY_PATH=. qemu-mips ./routerlocker
  ```

  Check behaviour of a MIPS binary,

  ```
  LD_LIBRARY_PATH=. qemu-mips -strace ./routerlocker
  ```

#### References

* https://people.debian.org/~aurel32/qemu/mips/

* https://gmplib.org/~tege/qemu.html

* http://toolchains.free-electrons.com/

* https://en.wikibooks.org/wiki/QEMU/Networking

* https://github.com/kholia/kernel-configs

#### Thanks

* rofl0r

* aurel32
