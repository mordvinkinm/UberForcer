uberforcer_windows : crypt.o queue.o parse_args.o bruteforce.o check.o network.o workers_network.o workers.o main.o
	gcc -o out/uberforcer.exe -O2 out/crypt3.o out/queue.o out/parse_args.o out/bruteforce.o out/check.o out/network.o out/workers_network.o out/workers.o out/main.o -lpthread -lws2_32

crypt.o : lib/crypt3.h lib/crypt3.c
	gcc -o out/crypt3.o -c lib/crypt3.c

queue.o : src/queue.h src/queue.c src/struct.h
	gcc -o out/queue.o -c src/queue.c

parse_args.o : src/parse_args.h src/parse_args.c
	gcc -o out/parse_args.o -c src/parse_args.c

bruteforce.o : src/bruteforce.h src/bruteforce.c
	gcc -o out/bruteforce.o -c src/bruteforce.c

check.o : src/check.h src/check.c
	gcc -o out/check.o -c src/check.c

network.o : src/network.h src/network.c
	gcc -o out/network.o -c src/network.c

workers_network.o : src/protocol.h src/workers_network.h src/workers_network.c
	gcc -o out/workers_network.o  -c src/workers_network.c

workers.o : src/workers.h src/workers.c
	gcc -o out/workers.o -c src/workers.c

main.o : src/main.c
	gcc -o out/main.o -c src/main.c