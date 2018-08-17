CC		= gcc
OUT		= out/uberforcer.exe
DEBUG	= 0
CFLAGS	= 


uberforcer_windows : crypt.o struct.o queue.o parse_args.o bruteforce.o check.o network.o workers_network.o workers.o main.o
	$(CC) $(LFLAGS) -o $(OUT) out/crypt3.o out/struct.o out/queue.o out/parse_args.o out/bruteforce.o out/check.o out/network.o out/workers_network.o out/workers.o out/main.o -lpthread -lws2_32

crypt.o : lib/crypt3.h lib/crypt3.c
	$(CC) $(CFLAGS) -o out/crypt3.o -c lib/crypt3.c -DDEBUG=$(DEBUG)

struct.o : src/struct.h src/struct.c
	$(CC) $(CFLAGS) -o out/struct.o -c src/struct.c -DDEBUG=$(DEBUG)

queue.o : src/queue.h src/queue.c src/struct.h
	$(CC) $(CFLAGS) -o out/queue.o -c src/queue.c -DDEBUG=$(DEBUG)

parse_args.o : src/parse_args.h src/parse_args.c
	$(CC) $(CFLAGS) -o out/parse_args.o -c src/parse_args.c -DDEBUG=$(DEBUG)

bruteforce.o : src/bruteforce.h src/bruteforce.c
	$(CC) $(CFLAGS) -o out/bruteforce.o -c src/bruteforce.c -DDEBUG=$(DEBUG)

check.o : src/check.h src/check.c
	$(CC) $(CFLAGS) -o out/check.o -c src/check.c -DDEBUG=$(DEBUG)

network.o : src/network.h src/network.c
	$(CC) $(CFLAGS) -o out/network.o -c src/network.c -DDEBUG=$(DEBUG)

workers_network.o : src/protocol.h src/workers_network.h src/workers_network.c
	$(CC) $(CFLAGS) -o out/workers_network.o  -c src/workers_network.c -DDEBUG=$(DEBUG)

workers.o : src/workers.h src/workers.c
	$(CC) $(CFLAGS) -o out/workers.o -c src/workers.c -DDEBUG=$(DEBUG)

main.o : src/main.c
	$(CC) $(CFLAGS) -o out/main.o -c src/main.c -DDEBUG=$(DEBUG)