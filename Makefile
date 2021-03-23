all: mytar

mytar: mytar.c
	gcc -ggdb -std=gnu99 -Wformat=0 -o mytar mytar.c

clean:
	rm mytar