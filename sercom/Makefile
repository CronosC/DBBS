SRC    = communication.c configureSerial.c buffer.c # source files
CFLAGS = -Wall
LFLAGS = 
OBJ    = $(SRC:.c=.o) # object files

# rule to compile C files
.c.o:
	gcc -c $(CFLAGS) $< -o $@

sercom: $(OBJ)
	gcc $(LFLAGS) $(OBJ) -o $@

clean:
	rm $(OBJ) sercom
