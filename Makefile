GCC=gcc

structure: structure.c
	$(GCC) -g -o structure $^

run: structure
	./structure

.PHONY:clean

clean:
	rm -f structure
