# UberForcer

Attempt to write a super-efficient bruteforcing tool for learning purposes on pure C.

# Implementation steps

1. [Done] Single-threading task
2. [Done] Multi-threading task
3. Client-server task, i.e. master & workers

# How to build
## Windows (using mingw make)
```
mingw32-make uberforcer_windows
```

## Windows (without make)
```
gcc -o uberforcer.exe -O2 lib/crypt3.h lib/crypt3.c src/main.c src/common.h src/config.h src/check.c src/check.h src/bruteforce.c src/bruteforce.h src/parse_args.h src/parse_args.c src/struct.h src/queue.h src/queue.c src/workers.h src/workers.c src/network.h src/network.c src/workers_network.h src/workers_network.c -lpthread -lws2_32
```

- Use -DDEBUG=1 key to build app with verbose info
- Use -DDEBUG=2 key to build app with trace info

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
