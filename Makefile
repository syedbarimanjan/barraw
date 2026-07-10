CC=gcc

main: main.c
	$(CC)  -g main.c -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 

run: main
	./main

clean:
	rm main

a:
	make clean && make main && make run

main-s: main.c
	$(CC) -fsanitize=address -g main.c -o main-s -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 

run-s: main
	LD_PRELOAD=/usr/lib64/libasan.so.8 ./main-s

clean-s:
	rm main-s

b:
	make clean-s && make main-s && make run-s