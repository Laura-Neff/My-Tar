all: mytar

mytar: mytar.c
	gcc -ggdb -std=c99 -Wformat=0 -o mytar mytar.c

clean:
	rm mytar