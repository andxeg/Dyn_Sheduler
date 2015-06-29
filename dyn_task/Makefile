all : dyn_sheduler clean

dyn_sheduler: main.o snapshot.o algorithm.o tinyxml2.o
	g++ ./main.o ./snapshot.o ./algorithm.o ./tinyxml2.o -o ./dyn_sheduler
	
main.o: ./src/main.cpp
	g++ -O2 -g -Wall -std=c++0x -c ./src/main.cpp -I ./include -I ./tinyxml2

snapshot.o: ./src/snapshot.cpp
	g++ -O2 -g -Wall -std=c++0x -c ./src/snapshot.cpp -I ./include -I ./tinyxml2
	
algorithm.o: ./src/algorithm.cpp
	g++ -O2 -g -Wall -std=c++0x -c ./src/algorithm.cpp -I ./include -I ./tinyxml2
	
tinyxml2.o: ./tinyxml2/tinyxml2.cpp
	g++ -O2 -g -Wall -std=c++0x -c ./tinyxml2/tinyxml2.cpp
clean:
	rm ./main.o ./snapshot.o ./algorithm.o ./tinyxml2.o
