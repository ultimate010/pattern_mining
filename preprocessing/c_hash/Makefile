all:
	cc -UDEBUG leak_detector_c.c readFile.c hash.c hashmap.c
debug:
	cc -DDEBUG leak_detector_c.c readFile.c hash.c hashmap.c -g
run:all
	./a.out
clean:
	rm -f a.out
