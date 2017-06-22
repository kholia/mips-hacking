#!/usr/bin/gdb -x
# Based on https://github.com/crossbowerbt/GDB-Python-Utils

import gdb
import traceback
import string
import subprocess


def usage():
    print("Usage: gdb -x ./gdb_bruteforce.py")
    gdb.execute('quit')


"""
mips, int v7 = i + 1; operation in routerlocker-clone.c

lw      $v0, 0x90+var_74($fp)
addiu   $v0, 1  <-- track this, 0x00400fb8
sw      $v0, 0x90+var_58($fp)
lw      $v0, 0x90+var_74($fp)
addiu   $v0, 1
sltiu   $v0, 1
andi    $s0, $v0, 0xFF
addiu   $v0, $fp, 0x90+var_34
move    $a0, $v0
la      $v0, strlen
move    $t9, $v0
jalr    $t9 ; strlen
"""

count = 0


class SnifferBreakpoint(gdb.Breakpoint):
    # Initialize the breakpoint
    def __init__(self):
        # super(SnifferBreakpoint, self).__init__('*0x400736')
        # super(SnifferBreakpoint, self).__init__('*0x555559c8')
        super(SnifferBreakpoint, self).__init__('*0x00400fb8')

    # Called when the breakpoint is hit
    def stop(self):
        global count
        try:
            # count = int(gdb.parse_and_eval('*(int*)($rbp - 0x4)'))
            # count = int(gdb.parse_and_eval('$v0'))
            count = int(gdb.parse_and_eval('$v0'))
            count = count + 1
        except:
            traceback.print_exc()
            return True

# GDB setup
gdb.execute("set print repeats unlimited")
gdb.execute("set print elements unlimited")
gdb.execute("set pagination off")
gdb.execute("set follow-fork-mode child")

known = ""

# generate sniffer breakpoint
SnifferBreakpoint()

for i in range(0, 29):
    count = 0
    # for c in "XYZ":
    for c in string.printable:
        # for c in "that":
        output = known + (c + "a" * (29 - len(known) - 1))
        print("Trying %s" % (output))
        with open("/tmp/router.lck", "w") as f:
            f.write(output)

        p = subprocess.Popen("qemu-mips -g 1234 ./routerlocker-clone", shell=True)
        gdb.execute('file routerlocker-clone')
        gdb.execute('target remote localhost:1234')
        gdb.execute('continue')
        if count > len(known):
            known = known + c
            break

gdb.execute('quit')
