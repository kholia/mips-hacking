#!/usr/bin/gdb -x
# Based on https://github.com/crossbowerbt/GDB-Python-Utils

import gdb
import traceback
import string


def usage():
    print("Usage: gdb -x ./gdb_bruteforce.py")
    gdb.execute('quit')


"""
amd64, i++ operation in app.c

0x0000000000400736 <+176>:	add    DWORD PTR [rbp-0x4],0x1
0x000000000040073a <+180>:	cmp    DWORD PTR [rbp-0x4],0x1c


mips, i++ operation in app.c

lw      v0,24(s8)
addiu   v0,v0,1


amd64, int v7 = i + 1; operation in routerlocker-clone.c

0x0000000000400cae <+1208>:	mov    eax,DWORD PTR [rbp-0x18]
0x0000000000400cb1 <+1211>:	add    eax,0x1
"""

count = 0


class SnifferBreakpoint(gdb.Breakpoint):
    # Initialize the breakpoint
    def __init__(self):
        # super(SnifferBreakpoint, self).__init__('*0x400736')
        # super(SnifferBreakpoint, self).__init__('*0x555559c8')
        super(SnifferBreakpoint, self).__init__('*0x400cb1')

    # Called when the breakpoint is hit
    def stop(self):
        global count
        try:
            # count = int(gdb.parse_and_eval('*(int*)($rbp - 0x4)'))
            # count = int(gdb.parse_and_eval('$v0'))
            count = int(gdb.parse_and_eval('$eax'))
            count = count + 1
        except:
            traceback.print_exc()
            return True

# GDB setup
gdb.execute("set print repeats unlimited")
gdb.execute("set print elements unlimited")
gdb.execute("set pagination off")
gdb.execute("set follow-fork-mode child")

# generate sniffer breakpoint
SnifferBreakpoint()

known = ""

for i in range(0, 29):
    count = 0
    # for c in "XYZ":
    for c in string.printable:
        # for c in "that":
        output = known + (c + "a" * (29 - len(known) - 1))
        print("Trying %s" % (output))
        with open("/tmp/router.lck", "w") as f:
            f.write(output)

        gdb.execute('file ./a.out')
        gdb.execute('run ./a.out')
        if count > len(known):
            known = known + c

gdb.execute('quit')
