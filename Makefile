CC = g++
CFLAGS = -Wall -g

main: main.o limiter.o
	$(CC) $(CFLAGS) -o main main.o limiter.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

limiter.o: src/limiter.cpp
	$(CC) $(CFLAGS) -c src/limiter.cpp -o limiter.o

clean:
	rm *.exe *.o
	rm -r obj -r bin
