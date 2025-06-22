.PHONY: build run clean

run:
	./tema2

build:
	gcc -Wall -Wextra main.c heap.c tree.c queue.c -g -o tema2

clean:
	rm -f tema2
