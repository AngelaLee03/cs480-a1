###########################################################
# CS480 Assignment #1
# Asynchronous Threads
# ###########################################################

EXEC = bots
CC = g++
LFLAGS = -pthread
CFLAGS = -c

$(EXEC): bots.cpp
	$(CC) bots.cpp -o $(EXEC) $(LFLAGS)

clean:
	rm -f $(EXEC) QUOTE.txt

#########################[ EOF: Makefile ]##################
