// Sample program to test GDB brute-forcing script

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
	unsigned char data[32];
	int i = 0;

	FILE *file = fopen("/tmp/router.lck", "r");
	if (file == NULL) {
		fwrite("License file not found.\n", 1, 24, stdout);
		fwrite("Lock it up, and lock it out.\n", 1, 29, stdout);
		return 1;
	}

	if (fread(data, 1, 29, file) >= 29) {
		fclose(file);

		while (1) {
			if (data[i] == 'X') {
				i++; // find out how many times this is getting hit!
			}
			else {
				break;
			}

			if (i >= 29)
				break;
		}

		if (i == 29) {
			puts("Success!");
			return 0;
		}

	}

	puts("Serial is invalid!");
	return 3;
}
