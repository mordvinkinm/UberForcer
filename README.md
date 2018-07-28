# UberForcer

Attempt to write a super-efficient bruteforcing tool for learning purposes on pure C.

# Implementation steps

1. [Done] Single-threading task
2. [Done] Multi-threading task
3. Client-server task, i.e. master & workers

# How to build
gcc -o uberforcer.exe -O2 main.c lib/crypt3.h lib/crypt3.c config.h common.h check.c check.h bruteforce.c bruteforce.h parse_args.h parse_args.c struct.h queue.h queue.c -lpthread

Use -DDEBUG=1 key to build app with verbose info
Use -DDEBUG=2 key to build app with trace info

Built and tested under windows 10, but the application should be cross-platform.

# How to run
Available commands:

uberforcer help                         show help file
uberforcer crypt <password> <salt>      call crypt function
uberforcer decrypt <hash> <args>        call decrypt function
uberforcer benchmark <args>             perform benchmarking

Available arguments:
-r or --recursive                       use recursive bruteforcing algorithm; mutually exclusive with --iterative
-i or --iterative                       [default] use iterative bruteforcing algorithm; mutually exclusive with --recursive
-a <value> or --alphabet <value>        available alphabet
-l <value> or --length <value>          presumed length of password
-t <value> or --threads <value>         number of threads for multithreading bruteforce

# Copyright
Thanks to Michael Dipperstein for original version of crypt code