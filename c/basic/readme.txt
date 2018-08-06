$ ulimit -c
$ ulimit -c 1024
add "-g" when compiling
$ ./segfault.o
will generate "core" file
$ gdb ./segfault.o core

