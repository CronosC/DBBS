#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

struct LineBuffer_str {
    char memory[BUFSIZE];           // buffer
    char *fillPtr;                  // fill pointer marks current insertion point
    void (*lineProcessor)(char *);  // function to call when \n is received
};

// init LineBuffer by resetting fill pointer and assigning a line processor (lp)
LineBuffer_p initBuffer(void (*lp)(char*)) {
    LineBuffer_p buf = (LineBuffer_p) malloc(sizeof(struct LineBuffer_str)); // allocate memory for buffer

    if (!buf) {
        // exit program if malloc() failed to allocate memory
        fprintf(stderr,"ERROR: Out of memory.\n");
        exit(1);
    }
    buf->fillPtr = buf->memory;
    buf->lineProcessor = lp;
    return buf;
}

void destroyBuffer(LineBuffer_p l) {
    free(l);
}

// read() data to line buffer, updating structure and triggering lineProcessor if line is complete
void lineBufferRead(LineBuffer_p buf, int device) {
    // read() data to buffer
    size_t bytesRead = read(device, buf->fillPtr, (buf->memory + BUFSIZE) - buf->fillPtr);
    // check whether line end was received
    while (bytesRead > 0) {
        if (*(buf->fillPtr) != '\n') {
            (buf->fillPtr)++;
            bytesRead--;
        } else {
            // line received: isolate string, call lineProcessor, and update buffer
            *(buf->fillPtr) = '\0';
            buf->lineProcessor(buf->memory);
            bytesRead--;
            memmove(buf->memory, (buf->fillPtr)+1, bytesRead); // data after \n to beginning of buffer
            (buf->fillPtr) = buf->memory;
        }
    }
    // check for overflow condition
    if (1 > (buf->memory + BUFSIZE) - buf->fillPtr) {
        fprintf(stderr, "Error: Line buffer overflow for device %d, resetting buffer.\n", device);
        buf->fillPtr = buf->memory;
    }
}
