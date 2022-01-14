
main:
	g++ -std=c++11 -Wall -Werr -o generate_name lastnames.o firstnames.o main.cpp

firstnames.h: names2c.py firstnames.txt
	py names2c.py firstnames firstnames.txt firstnames.h

lastnames.h: names2c.py lastnames.txt
	py names2c.py lastnames lastnames.txt lastnames.h

firstnames.o: firstnames.h firstnames.cpp
	g++ -std=c++11 -Wall -Werr -o firstnames.o firstnames.cpp -c

lastnames.o: firstnames.h firstnames.cpp
	g++ -std=c++11 -Wall -Werr -o lastnames.o lastnames.cpp -c

clean:
	rm firstnames.o
	rm lastnames.o
	rm firstnames.h
	rm lastnames.h
	rm generate_name
