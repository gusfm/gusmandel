CC = gcc
CFLAGS = -O3 -Wall
LDLIBS = -lm
PROG = gusmandel

OBJS = 	image.o \
		palette.o \
		mandel.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDLIBS)

.PHONY : clean

clean :
	-rm $(PROG) $(OBJS)

