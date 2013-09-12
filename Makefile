CC=gcc
WANDFLAG=`Wand-config --cflags --cppflags`
WANDCONFIG=`Wand-config --ldflags --libs`
XMLFLAG=`xml2-config --cflags --libs`
CFLAGS = -W -Wall -Wextra -O2
CFILES= src/main.c
PROGS=web_server
PREFIX=/opt/tvs/
BIN=/usr/bin/

all:
	$(CC) $(CFLAGS) $(WANDFLAG) $(CFILES) $(WANDCONFIG) $(XMLFLAG) -o $(PROGS) -pthread

debug:
	$(CC) $(CFLAGS) $(WANDFLAG) -g $(CFILES) $(WANDCONFIG) $(XMLFLAG) -o $(PROGS) -pthread

clean:
	rm -f $(PROGS) *.o

install:
	sudo mkdir $(PREFIX)
	sudo cp -v -t $(PREFIX) server.conf wurfl.xml mime.types $(PROGS)
	sudo cp -v -t $(BIN) server-ctl
	sudo chmod +x $(BIN)/server-ctl
	sudo chmod 777 $(PREFIX)

unistall:
	sudo rm -v -rf $(PREFIX)*
	sudo rmdir -v $(PREFIX)
	sudo rm -v $(BIN)/server-ctl 
