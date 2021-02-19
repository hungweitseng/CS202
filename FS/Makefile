OBJS=get_sector 
LIBS=-lpthread
CFLAGS=-g -O3 -DDEBUG

all: $(OBJS)

get_sector: get_sector.c
	$(CC) $(CFLAGS) get_sector.c -o get_sector $(LIBS)

read_nvme: read_nvme.c
	$(CC) $(CFLAGS) read_nvme.c -o read_nvme $(LIBS)

clean:
	rm -f $(OBJS)
	rm -rf *.dSYM
	rm -rf *~
