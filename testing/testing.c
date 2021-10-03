#include <stdio.h>

int str_equal(char a[], char b[]) {
	int i = 0;
	int is_equal = 1;
	while(1) {
		if((a[i] != b[i])) {
			is_equal = 0;
		}
		if((a[i] == '\n') || (a[i] == 0x00)) {
			break;
		}
		i++;
	}
	
	return is_equal;
}


int main(int argc, char **argv){
	char x[32];
	x[0] = 'p';
	x[1] = 'i';
	x[2] = 'n';
	x[3] = 'g';
	x[4] = '\n';
	x[5] = 0x00;

	printf("'%i' \n", str_equal(x, "ping\n"));
}
