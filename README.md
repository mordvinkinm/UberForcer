# UberForcer

Attempt to write a super-efficient bruteforcing tool for learning purposes on pure C.

# Implementation steps

1. [Done] Single-threading task
2. [Done] Multi-threading task
3. Client-server task, i.e. master & workers

# How to build
Windows:
gcc -o uberforcer.exe -O2 main.c lib/crypt3.h lib/crypt3.c common.h config.h network.h check.c check.h bruteforce.c bruteforce.h parse_args.h parse_args.c struct.h queue.h queue.c workers.h workers.c workers_network.h workers_network.c -lpthread -lws2_32

Linux:
gcc -o uberforcer.exe -O2 main.c lib/crypt3.h lib/crypt3.c common.h config.h network.h check.c check.h bruteforce.c bruteforce.h parse_args.h parse_args.c struct.h queue.h queue.c workers.h workers.c workers_network.h workers_network.c -lpthread

Use -DDEBUG=1 key to build app with verbose info
Use -DDEBUG=2 key to build app with trace info

Built and tested under windows 10, but the application should be cross-platform.

# How to run
Available commands:

uberforcer help                                 show help file
uberforcer crypt <password> <salt>              encrypt provided <password>, using provided <salt>
uberforcer decrypt <hash> <args>                bruteforce provided <hash>, using additional arguments <args> [optional]. available arguments: -r, -i, -a, -l, -t
uberforcer benchmark <args>                     perform benchmarking, using bruteforcing arguments <args> [optional]. available arguments: -r, -i, -a, -l, -t
uberforcer server <port> <args>                 start bruteforcing server on the provided port, using bruteforcing arguments <args> [optional]. available arguments: -r, -i, -a, -l
uberforcer client <host> <port> <args>          start bruteforcing client to connect to server on <host>:<port>, using bruteforcing arguments <args> [optional]. available arguments: -r, -i, -t

Available <args> arguments:
-r or --recursive                               use recursive bruteforcing algorithm; mutually exclusive with --iterative
-i or --iterative                               [default] use iterative bruteforcing algorithm; mutually exclusive with --recursive
-a <value> or --alphabet <value>                available alphabet; default: [A-Za-z0-9]
-l <value> or --length <value>                  presumed length of password; default: 4
-t <value> or --threads <value>                 Number of threads for multithreading bruteforce; default: 1

# Copyright
Thanks to Michael Dipperstein for original version of crypt code