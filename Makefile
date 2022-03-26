CFLAGS = -g -Wall -Werror -O3
CC = g++

PROGRAM = find

 all: $(PROGRAM)
 
  $(PROGRAM): $(PROGRAM).cc
	$(CC) $(CFLAGS) -o $(PROGRAM) $(PROGRAM).cc
 
  clean:
	$(RM) $(PROGRAM)