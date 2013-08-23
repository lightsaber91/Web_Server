CC=gcc
WANDFLAG=`Wand-config --cflags --cppflags`
WANDCONFIG=`Wand-config --ldflags --libs`
XMLFLAG=`xml2-config --cflags --libs`
CFLAGS = -W -Wall -Wextra -O2
CFILES= src/main.c
PROGS=server
PREFIX=/opt/tvs/
BIN=/usr/bin/

all:
	$(CC) $(CFLAGS) $(WANDFLAG) $(CFILES) $(WANDCONFIG) $(XMLFLAG) -o $(PROGS) -pthread

clean:
	rm -f $(PROGS) *.o

install:
	sudo mkdir $(PREFIX)
	sudo cp -v -t $(PREFIX) server.conf wurfl.xml mime.types
	sudo cp -v -t $(BIN) $(PROGS) server-ctl

unistall:
	sudo rm -v -rf $(PREFIX)*
	sudo rmdir -v $(PREFIX)
	sudo rm -v $(BIN)/server-ctl $(BIN)/server
