CFLAGS=-Wall -std=c99 -g

all: srv clt

.PHONY: clean
clean:
	rm -f srv clt

srv: srv.c
	$(CC) $(CFLAGS) -o $@ $^ 

clt: clt.c
	$(CC) $(CFLAGS) -o $@ $^ 
