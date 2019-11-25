GCC=gcc

structure: structure.c
	$(GCC) -o structure $^

run: structure
	./structure

.PHONY:clean

clean:
	rm -f structure
