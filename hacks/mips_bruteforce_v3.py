#!/usr/bin/gdb -x
# Based on https://github.com/crossbowerbt/GDB-Python-Utils

import gdb
import time
import traceback
# import string
import subprocess


def usage():
    print("Usage: gdb -x ./mips_bruteforce_v3.py")
    gdb.execute('quit')


"""
Idea: strlen is called in the "goodboy" code path and each call instance
indicates a good input character. So count the number of calls to strlen.

loc_400D10:
addiu   $v0, $fp, 0x98+var_34
move    $a0, $v0
jal     strlen  <-- track hit to this, 0x400d18
move    $at, $at
move    $s1, $v0
move    $s0, $zero
lw      $v0, 0x98+var_80($fp)
sltu    $v0, $s0
bnez    $v0, loc_400C18
move    $at, $at
"""

count = 0


class SnifferBreakpoint(gdb.Breakpoint):
    # Initialize the breakpoint
    def __init__(self):
        super(SnifferBreakpoint, self).__init__('*0x400d18')

    # Called when the breakpoint is hit
    def stop(self):
        global count
        try:
            count = count + 1
        except:
            traceback.print_exc()
            return True

# GDB setup
gdb.execute('file routerlocker.patched')
# gdb.execute("set print repeats unlimited")  # not supported in older gdb
# gdb.execute("set print elements unlimited")  # ditto
gdb.execute("set pagination off")
gdb.execute("set confirm off")
gdb.execute("set architecture mips")
gdb.execute("set follow-fork-mode child")

known = ""

# generate sniffer breakpoint
SnifferBreakpoint()

for i in range(0, 30):
    count = 0
    for c in "etaoinshrdlcumwfgypbvkjxqz_0345126789ETAOINSHRDLCUMWFGYPBVKJXQZ":  # rofl0r
        # for c in string.printable:
        count = 0
        output = known + (c + "a" * (29 - len(known) - 1))
        print("Trying %s" % (output))
        with open("/tmp/router.lck", "w") as f:
            f.write(output)

        p = subprocess.Popen("LD_LIBRARY_PATH=. qemu-mips -g 1234 ./routerlocker.patched", shell=True)
        time.sleep(0.02)  # rofl0r
        gdb.execute('target remote localhost:1234')
        gdb.execute('continue')
        # the first strlen call is done even if the input is totally wrong,
        # "count - 1" is done to skip this particular strlen call.
        if count - 1 > len(known):
            known = known + c
            break

gdb.execute('quit')
