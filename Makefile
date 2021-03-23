# // THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
# // A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LAURA NEFF

all: mytar

mytar: mytar.c
	gcc -ggdb -std=c99 -Wformat=0 -o mytar mytar.c

clean:
	rm mytar