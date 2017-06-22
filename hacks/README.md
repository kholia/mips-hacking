#### Patching notes

Patching `ptrace` call,

```
$ sed -i 's|ptrace|isnanl|' routerlocker
```

For patching `fork` call, replace `jal fork@plt` instruction bytes with `move v0, zero` instruction bytes.

```
$ mips-linux-objdump -d routerlocker
...
4008d4:       0c1001d4        jal     400750 <fork@plt>
4008ec:       00001021        move    v0,zero
```

### QEMU debugging

```
$ LD_LIBRARY_PATH=. qemu-mips -g 1234 ./routerlocker.patched
```

#### QEMU easier userspace emulation

```
$ sudo dnf install -C qemu-user-binfmt -y

$ sudo service systemd-binfmt restart

$ LD_LIBRARY_PATH=. ./ld.so.1 ./routerlocker
```

### Build GDB-with-Python for MIPS

Fetch and extract `gdb-8.0.tar.xz`.

```
$ sudo dnf builddep gdb
```

```
$ targets="--enable-targets=x86_64-linux,i386-linux,powerpc-linux,arm-linux,mips-linux,mipsel-linux"
$ ./configure --enable-64-bit-bfd $targets
$ make
$ make install
```

```
$ cat ~/.gdbinit
set history save on
set print pretty on
set pagination off
set confirm off

set follow-fork-mode child
```

```
$ /usr/local/bin/gdb ./routerlocker
```

#### References

* http://chortle.ccsu.edu/assemblytutorial/

#### Thanks

* rofl0r

* csec
