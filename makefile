uberforcer_windows : crypt.o queue.o parse_args.o bruteforce.o check.o network.o workers_network.o workers.o main.o
	gcc -o out/uberforcer.exe -O2 out/crypt3.o out/queue.o out/parse_args.o out/bruteforce.o out/check.o out/network.o out/workers_network.o out/workers.o out/main.o -lpthread -lws2_32

crypt.o : lib/crypt3.h lib/crypt3.c
	gcc -o out/crypt3.o -c lib/crypt3.c

queue.o : queue.h queue.c struct.h
	gcc -o out/queue.o -c queue.c

parse_args.o : parse_args.h parse_args.c
	gcc -o out/parse_args.o -c parse_args.c

bruteforce.o : bruteforce.h bruteforce.c
	gcc -o out/bruteforce.o -c bruteforce.c

check.o : check.h check.c
	gcc -o out/check.o -c check.c

network.o : network.h network.c
	gcc -o out/network.o -c network.c

workers_network.o : protocol.h workers_network.h workers_network.c
	gcc -o out/workers_network.o  -c workers_network.c

workers.o : workers.h workers.c
	gcc -o out/workers.o -c workers.c

main.o : main.c
	gcc -o out/main.o -c main.c