all:
# Linux x86 32bit only for now
	@gcc -m32 -g -c --std=c11 ./src/*.c -I ./include -Wall -O0
	@gcc -m32 -ldl -lm -o voxp *.o

clean:
	@rm -fv *.o
