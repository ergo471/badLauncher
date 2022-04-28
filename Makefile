CC = g++
CFLAGS = -Wall -g -std=c++11

main: main.o limiter.o
	$(CC) $(CFLAGS) -o main main.o limiter.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

limiter.o: include/limiter.h src/limiter.cpp
	$(CC) $(CFLAGS) -c src/limiter.cpp -o limiter.o

clean:
	rm *.exe *.o
	rm -r obj -r bin
