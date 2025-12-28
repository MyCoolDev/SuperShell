cc = gcc

build:
	cc main.c -o ../build/main
dev:
	cc -fsanitize=address -g main.c -o ../build/main
	../build/main
