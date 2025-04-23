.PHONY: clean

CFLAGS=-Wall -g

driver: driver.o rbtree.o

driver.o : driver.c
	gcc -c driver.c

rbtree.o : rbtree.c
	gcc -c rbtree.c

clean:
	rm -f driver *.o
