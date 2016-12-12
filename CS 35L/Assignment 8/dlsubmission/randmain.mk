CC = gcc
CFLAGS = -O2 -g3 -Wall -Wextra -march=native -mtune=native -mrdrnd

randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) -fPIC -c randlibhw.c -o randlibhw.o
	$(CC) $(CFLAGS) -shared -o randlibhw.so randlibhw.o
randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) -fPIC -c randlibsw.c -o randlibsw.o
	$(CC) $(CFLAGS) -shared -o randlibsw.so randlibsw.o

randmain: randcpuid.o randmain.o
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) randmain.o randcpuid.o -o randmain 	
