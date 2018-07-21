# UberForcer

Attempt to write a super-efficient bruteforcing tool for learning purposes on pure C.

# Implementation steps

1. Single-threading task
2. Multi-threading task
3. Client-server task, i.e. master & workers

# How to build

gcc -o uberforcer.exe -O2 main.c crypt3.h crypt3.c config.h common.h check.c check.h bruteforce.c bruteforce.h

Add -DDEBUG=1 key to build app with verbose info
Add -DDEBUG=2 key to build app with trace info