
# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11 -Iinclude
LFLAGS  = -lboost_system -pthread

# All Targets
all: StompBookClubClient

# Tool invocations
StompBookClubClient: bin/StompBookClubClient.o bin/connectionHandler.o bin/User.o bin/Frame.o bin/BookInfo.o bin/main.o
	@echo 'Building target: StompBookClubClient'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/StompBookClubClient bin/StompBookClubClient.o bin/connectionHandler.o bin/User.o bin/Frame.o bin/BookInfo.o bin/main.o $(LFLAGS)
	@echo 'Finished building target: StompBookClubClient'
	@echo ' '

# Depends on the source and header files
bin/BookInfo.o: src/BookInfo.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/BookInfo.o src/BookInfo.cpp
bin/connectionHandler.o: src/connectionHandler.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/connectionHandler.o src/connectionHandler.cpp
bin/StompBookClubClient.o: src/StompBookClubClient.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/StompBookClubClient.o src/StompBookClubClient.cpp
bin/Frame.o: src/Frame.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Frame.o src/Frame.cpp
bin/User.o: src/User.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/User.o src/User.cpp
bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/main.o src/main.cpp
#Clean the build directory
clean:
	rm -f bin/*
