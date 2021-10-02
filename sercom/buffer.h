//
//  buffer.h
//  
//
//  Implements a buffer data structure that reads in text and invokes
//  a processing function whenever a complete \n-terminated line is read
//

#ifndef __buffer__
#define __buffer__

#define BUFSIZE 1024

struct LineBuffer_str; // forward declaration to hide contents
typedef struct LineBuffer_str *LineBuffer_p;

// constructor, takes 'lineProcessor' function as argument, returns pointer type
// lineProcessor must be of type "void someFunction(char* lineToProcess);"
LineBuffer_p initBuffer( void (*lineProcessor)(char*));

// destructor
void destroyBuffer(LineBuffer_p l);

// read to buffer (requires non-blocking I/O on 'device')
void lineBufferRead(LineBuffer_p buf, int device);

#endif /* defined(__buffer__) */
