all:
	gcc -g -c --std=c11 ./src/*.c -I ./include -Wall -O3 -march=native
	gcc -ldl -lm ./*.o -o voxp

debug:
	gcc -g -c --std=c11 ./src/*.c -I ./include -Wall -O0
	gcc -ldl -lm ./*.o -o voxp

clean:
	rm -fv *.o voxp *.wav
