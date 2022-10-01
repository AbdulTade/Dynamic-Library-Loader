CC=g++.exe
CFLAGS=-g -Wall

loader.exe: DynLoader.cpp test.cpp
	$(CC) $(CFLAGS) -o loader.exe test.cpp DynLoader.cpp 

clean:
	rm *.exe