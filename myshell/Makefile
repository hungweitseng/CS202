OBJS = myshell noise

all: $(OBJS)

myshell: myshell.c
	$(CC) -g -O3 myshell.c -o myshell

noise: noise.c
	$(CC) -g -O3 noise.c -o noise

clean:
	rm -f $(OBJS)
