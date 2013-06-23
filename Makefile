CC=gcc
WANDFLAG=`Wand-config --cflags --cppflags`
WANDCONFIG=`Wand-config --ldflags --libs`
XMLFLAG=`xml2-config --cflags --libs`
CFLAGS = -W -Wall -Wextra -O2
CFILES= src/main.c
PROGS=main

all:$(PROGS)
	$(CC) $(CFLAGS) $(WANDFLAG) $(CFILES) $(WANDCONFIG) $(XMLFLAG) -o $^ -pthread

clean:
	rm -f $(PROGS) *.o
