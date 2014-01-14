CFLAGS=-c -std=c++0x -g
LDFLAGS=-lpthread -lboost_thread -lboost_system

all: fakesource

fakesource: fakesource.o
	g++ -o fakesource fakesource.o $(LDFLAGS) 

fakesource.o: fakesource.cpp
	g++ $(CFLAGS) fakesource.cpp

fakesource_mac: fakesource.cpp
	clang++ -o fakesource -Wall -ansi -pedantic -O3 fakesource.cpp $(LDFLAGS) -std=c++11 -stdlib=libc++ -I /opt/local/include -L /opt/local/lib

clean:
	rm *.o
