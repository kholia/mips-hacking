#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

char runtime_pad[] =
"\xB6\xA5\x02\xB6\xDD\x73\x37\x35\x5F\x23\xA5\xC9\xB0\x8D\xE9\xAB\x81\xA3\xDF\x0C"
"\x80\xAF\xB4\x7D\x63\x79\xF4\x5C\x4F\x16\x13\xC3\xFB\xBE\x5E\x30\x22\x2E\x82\x66"
"\x1B\xDE\x67\xC1\x99\x4C\xBE\x23\x44\xB4\x7B\xF5\xE7\x2C\x41\x69\xD1\x78\xDF\xDB"
"\x45\x5B\xCF\x49\x74\x07\x13\x8F\x40\x18\xE7\xE6\xCE\xFA\x56\x37\xFC\x74\x33\x81"
"\xE3\xDC\xA4\x0C\x44\x80\x57\xBA\x3E\xF9\xD9\xA7\xB4\x5E\xCD\xA5\xEB\x44\x5B\xEF"
"\x46\xF3\x5B\x6D\xCF\xE0\x38\xA7\xC7\xB8\x6A\x18\x16\xE1\xC5\x03\x1E\x18\x52\xDE"
"\x84\xF6\x27\xF0\x60\x2D\x27\x4B";


int main(int argc, char ** argv) {
	int stat_loc = 0;
	if (fork() != 0) {
		wait(&stat_loc);
		return 0;
	}
	ptrace(0);
	FILE * file = fopen("/tmp/router.lck", "r"); // 0x400a1c
	if (file == NULL) {
		fwrite("License file not found.\n", 1, 24, stdout);
		fwrite("Lock it up, and lock it out.\n", 1, 29, stdout);
		return 1;
	}
	int v0, zero = 0, fp = -68;
	char byte[29];
	v0 = zero - 0x78;
	byte[fp + 0x4c] = v0;
	v0 = zero + 0xa;
	byte[fp + 0x4e] = v0;
	v0 = zero - 0x21;
	byte[fp + 0x47] = v0;
	v0 = zero - 0x70;
	byte[fp + 0x58] = v0;
	v0 = zero - 0x22;
	byte[fp + 0x48] = v0;
	v0 = zero + 0x79;
	byte[fp + 0x55] = v0;
	v0 = zero - 0x46;
	byte[fp + 0x52] = v0;
	v0 = zero + 0x25;
	byte[fp + 0x50] = v0;
	v0 = zero - 0x64;
	byte[fp + 0x5c] = v0;
	v0 = zero - 0x4c;
	byte[fp + 0x5a] = v0;
	v0 = zero + 0x16;
	byte[fp + 0x53] = v0;
	v0 = zero - 0x3e;
	byte[fp + 0x44] = v0;
	v0 = zero + 0x6a;
	byte[fp + 0x5f] = v0;
	v0 = zero - 0x11;
	byte[fp + 0x59] = v0;
	v0 = zero - 0x6b;
	byte[fp + 0x4a] = v0;
	v0 = zero + 0x54;
	byte[fp + 0x4f] = v0;
	v0 = zero + 0x5d;
	byte[fp + 0x5e] = v0;
	v0 = zero + 0x73;
	byte[fp + 0x60] = v0;
	v0 = zero - 0x53;
	byte[fp + 0x4b] = v0;
	v0 = zero + 0x1b;
	byte[fp + 0x45] = v0;
	v0 = zero + 0x5e;
	byte[fp + 0x51] = v0;
	v0 = zero - 0x22;
	byte[fp + 0x57] = v0;
	v0 = zero - 0x40;
	byte[fp + 0x5b] = v0;
	v0 = zero + 6;
	byte[fp + 0x54] = v0;
	v0 = zero - 0x23;
	byte[fp + 0x49] = v0;
	v0 = zero + 0x41;
	byte[fp + 0x4d] = v0;
	v0 = zero - 0x3c;
	byte[fp + 0x46] = v0;
	v0 = zero - 0x65;
	byte[fp + 0x5d] = v0;
	v0 = zero + 0x38;
	byte[fp + 0x56] = v0;

	char data[30] = {0};
	int result;
	if (fread(data, 1, 29, file) >= 29) {
		fclose(file);
		if (strlen(data) != 0) {
			int i = 0; // 0x400d484857
			while (true) {
				int j = ((i << 2) & 0xff) + (i & 3);
				if (byte[i] != (data[i] ^ runtime_pad[j])) {
					fwrite("Serial is invalid.\n", 1, 19, stdout);
					fwrite("Ambrocious, unlock this door!\n", 1, 30, stdout);
					return 3;
				}
				int v7 = i + 1;
				if (i == -1 | v7 >= strlen(data)) {
					break;
				}
				i = v7;
			}
		}
		puts("Thank you for purchasing RouterLocker v2.0");
		printf("Your flag is: PAN{%s}\n", data);
		result = 0;
	} else {
		fwrite("License file invalid.\n", 1, 22, stdout);
		fwrite("Lock it up, and lock it out.\n", 1, 29, stdout);
		result = 2;
	}
	return result;
}
