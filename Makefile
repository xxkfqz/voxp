all:
	gcc -g -c --std=c11 ./src/*.c -I ./include -Wall -O3 -pipe
	gcc -ldl -lm -o voxp *.o

clean:
	rm -fv *.o
