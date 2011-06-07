CC = gcc
CFLAGS = -O3 -Wall
LDLIBS = -lm
PROG = gusmandel

OBJS = image.o \
       mandel.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDLIBS)

.PHONY : clean

clean :
	-rm $(PROG) $(OBJS)

