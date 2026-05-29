bots: bots.cpp
	g++ bots.cpp -o bots -pthread

clean:
	rm -f bots QUOTE.txt
