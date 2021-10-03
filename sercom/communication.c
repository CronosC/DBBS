// communication with serial device
//
// USAGE: sercom <SERIAL_DEVICE> <BAUD_RATE>
// serial device is a path like /dev/ttysAC0 on Linux or /dev/cu.usbmodem1411 on MacOS or COM1 on Windows/Cygwin
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

#include "configureSerial.h"
#include "buffer.h"

int gRunning = 1; // global flag: set to 0 when program shall quit

// handle a line of input from serial (just print to stdout)
void processData(char *line) {
  while (*line) {
        // print out character by character, replacing non-printable ones
        if (isprint(*line)) {
            putchar(*line);
        }
        line++;
    }
  

}

void processUserData(char *line) {
	if(*line == (int) 'q' && *(line+1) == 0x00) {
		gRunning = 0;
	}
}

// arguments:
// argv[1] is the given path to the device location
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "USAGE: %s <SERIAL_DEVICE>\n, <BAUD_RATE>", argv[0]);
        return EXIT_FAILURE;
    }

    // open serial device ("our board") for read-write and configure to 57600 8N1
    int serialDevice = open(argv[1], O_RDWR);
    if (serialDevice < 0) {
        perror("opening serial device failed");
        close(serialDevice);
        return EXIT_FAILURE;
    }
    serialDevice = configureSerial(serialDevice, atoi(argv[2]));
    if (serialDevice < 0) {
        perror("configuring serial device failed");
        close(serialDevice);
        return EXIT_FAILURE;
    }

	// set up file set for select() to wait for incoming data
    fd_set inputs;
    int fd_max = serialDevice+1;

    // init a buffer to hold complete line of data from board
    LineBuffer_p serialBuffer = initBuffer(&processData);
    LineBuffer_p userBuffer = initBuffer(&processUserData);

    //printf("baud rate set to: %s \n", argv[2]);
    printf("listening on %s, enter 'q' + enter to quit...\n", argv[1]);



	while (gRunning) {
		// set up the input set
		FD_ZERO(&inputs); // <---- HINT: FD_ZERO and FD_SET are explained on the man page for the "select" function
		FD_SET(serialDevice, &inputs);
		FD_SET(0, &inputs);

		// set up a time-out (5 secs)
		struct timeval time;
		time.tv_sec = 15;  // 'full' seconds
		time.tv_usec = 0; // microseconds

		// wait/check for incoming data
		if(select(fd_max, &inputs, NULL, NULL, &time) > 0) {  // <--- HINT: consult man page for select() to figure out how to use it!
		    // we got data!
		    // if serial device has sent something, read it!
		    if (FD_ISSET(serialDevice, &inputs)) {
		        lineBufferRead(serialBuffer, serialDevice);
		    }

		    // 0 is stdin
		    if (FD_ISSET(0, &inputs)) {
		        lineBufferRead(userBuffer, 0);
		        write(serialDevice, userBuffer, sizeof(userBuffer) - 1);
		    }


		} else {
		    printf("Not data received for 15 seconds.\n");
		}
    }
    printf("Goodbye!\n");
    close(serialDevice);
    destroyBuffer(serialBuffer);
    destroyBuffer(userBuffer);
    return EXIT_SUCCESS;
}
