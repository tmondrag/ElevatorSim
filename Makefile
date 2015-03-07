CC = g++
CFLAGS = -g -Wall

all: PersonTest PersonQueueTest

PersonTest: PersonTest.o person.o
	$(CC) PersonTest.o person.o -o PersonTest

PersonQueueTest: person.o PersonQueueTest.o personqueue.o
	$(CC) person.o PersonQueueTest.o personqueue.o -o PersonQueueTest -lpthread

person.o: person.cpp
	$(CC) $(CFLAGS) person.cpp -c -o person.o

PersonTest.o: PersonTest.cpp
	$(CC) $(CFLAGS) PersonTest.cpp -c -o PersonTest.o

personqueue.o: personqueue.cpp
	$(CC) $(CFLAGS) personqueue.cpp -c -o personqueue.o 

PersonQueueTest.o: PersonQueueTest.cpp
	$(CC) $(CFLAGS) PersonQueueTest.cpp -c -o PersonQueueTest.o 

clean:
	rm *.o *~

uninstall:
	rm *.o PersonTest PersonQueueTest *~

