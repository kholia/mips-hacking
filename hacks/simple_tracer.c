/* Code sample: using ptrace for simple tracing of a child process.
**
** Eli Bendersky (http://eli.thegreenplace.net)
** This code is in the public domain.
*/
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#if !defined(__mips__)
#include <sys/reg.h>
#endif
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>

// from google-breakpad and http://elixir.free-electrons.com/linux/latest/source/arch/mips/include/uapi/asm/ptrace.h
#if defined(__mips__)
struct user_regs_struct {
	uint64_t regs[32];
	uint64_t lo;
	uint64_t hi;
	uint64_t epc;
	uint64_t badvaddr;
	uint64_t status;
	uint64_t cause;
};
#endif

/*
 * Print a message to stdout, prefixed by the process ID
 */
void procmsg(const char* format, ...)
{
	va_list ap;
	fprintf(stdout, "[%d] ", getpid());
	va_start(ap, format);
	vfprintf(stdout, format, ap);
	va_end(ap);
}


void run_target(const char* programname)
{
	procmsg("target started. will run '%s'\n", programname);

	/* Allow tracing of this process */
	if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
		perror("ptrace run_target");
		return;
	}

	/* Replace this process's image with the given program */
	execl(programname, programname, 0);
}


void run_debugger(pid_t child_pid)
{
	int wait_status;
	unsigned icounter = 0;
	procmsg("debugger started\n");

	/* Wait for child to stop on its first instruction */
	wait(&wait_status);

	while (WIFSTOPPED(wait_status)) {
		icounter++;
		struct user_regs_struct regs;
		ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
		// unsigned instr = ptrace(PTRACE_PEEKTEXT, child_pid, regs.rip, 0);
		// procmsg("icounter = %u.  EIP = 0x%08x.  instr = 0x%08x\n", icounter, regs.rip, instr);
#if defined(__mips__)
		procmsg("icounter = %u.  EIP = 0x%08x\n", icounter, regs.epc);
#else
		procmsg("icounter = %u.  EIP = 0x%08x\n", icounter, regs.rip);
#endif
		// http://elixir.free-electrons.com/linux/latest/source/arch/mips/kernel/ptrace.c,
		// Linux/MIPS does not support PTRACE_SINGLESTEP because there
		// is no single step instruction feature on most MIPS processors.

		/* Make the child execute another instruction */
		if (ptrace(PTRACE_SINGLESTEP, child_pid, 0, 0) < 0) {
			perror("ptrace run_debugger");
			return;
		}

		/* Wait for child to stop on its next instruction */
		wait(&wait_status);
	}

	procmsg("the child executed %u instructions\n", icounter);
}


int main(int argc, char** argv)
{
	pid_t child_pid;

	if (argc < 2) {
		fprintf(stderr, "Expected a program name as argument\n");
		return -1;
	}

	child_pid = fork();
	if (child_pid == 0)
		run_target(argv[1]);
	else if (child_pid > 0)
		run_debugger(child_pid);
	else {
		perror("fork");
		return -1;
	}

	return 0;
}
