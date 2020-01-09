all: virtualization

virtualization: virtualization.c
	$(CC) -g virtualization.c -o virtualization
	objdump -D virtualization > virtualization.dump
clean:
	rm -f virtualization
