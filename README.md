# UberForcer

Attempt to write a super-efficient bruteforcing tool for learning purposes on pure C.

# Implementation steps

1. [Done] Single-threading task
2. [Done] Multi-threading task
3. [Done] Client-server task, i.e. master & workers

# How to build
## Windows (using mingw make)
<pre>
mingw32-make uberforcer_windows
</pre>

- Use CLAGS="..." to pass compilation flags (for example, CFLAGS="-O2")
- Use LFLAGS="..." to pass linker optiopns
- Use OUT="..." to specify output path (for example, OUT="out/uberforcer.exe")
- Use DEBUG=1 make key to build app with verbose info
- Use DEBUG=2 make key to build app with trace info

## Windows (without make)
If you don't have make tool for some reason, here is gcc cmd command:
<pre>
gcc -o uberforcer.exe -O2 lib/crypt3.h lib/crypt3.c src/main.c src/common.h src/config.h src/check.c src/check.h src/bruteforce.c src/bruteforce.h src/parse_args.h src/parse_args.c src/struct.h src/struct.c src/queue.h src/queue.c src/workers.h src/workers.c src/network.h src/network.c src/workers_network.h src/workers_network.c -lpthread -lws2_32
</pre>

- Use -DDEBUG=1 compilation key to build app with verbose info
- Use -DDEBUG=2 compilation key to build app with trace info

Built and tested under windows 10, but the application should be cross-platform.

# How to run
<pre>
Available commands:  

uberforcer help                          show help file  
uberforcer crypt $password $salt         encrypt provided $password, using provided $salt
uberforcer decrypt $hash $args           bruteforce provided $hash, using additional arguments $args [optional]. available arguments: -r, -i, -a, -l, -t
uberforcer benchmark $args               perform benchmarking, using bruteforcing arguments $args [optional]. available arguments: -r, -i, -a, -l, -t
uberforcer server $hash $port $args      start bruteforcing server on the provided port, to bruteforce $hash, using bruteforcing arguments $args [optional]. available arguments: -r, -i, -a, -l
uberforcer client $host $port $args      start bruteforcing client to connect to server on $host:$port, using bruteforcing arguments $args [optional]. available arguments: -r, -i, -t

Available $args arguments:
-r or --recursive                        use recursive bruteforcing algorithm; mutually exclusive with --iterative
-i or --iterative                        [default] use iterative bruteforcing algorithm; mutually exclusive with --recursive
-a $value or --alphabet $value           available alphabet; default: [A-Za-z0-9]
-l $value or --length $value             presumed length of password; default: 4
-t $value or --threads $value            number of threads for multithreading bruteforce; default: 1
</pre>
  
# Copyright
Thanks to Michael Dipperstein for original version of crypt3 code
