enigma: main.o utils.o PlugBoard.o Reflector.o Rotor.o RotorPositions.o
	g++ -g main.o utils.o PlugBoard.o Reflector.o Rotor.o RotorPositions.o -o enigma
main.o: main.cpp utils.h PlugBoard.h Reflector.h Rotor.h RotorPositions.h
	g++ -g -Wall -c main.cpp
utils.o: utils.cpp utils.h PlugBoard.h Reflector.h Rotor.h RotorPositions.h
	g++ -g -Wall -c utils.cpp
PlugBoard.o: PlugBoard.cpp utils.h PlugBoard.h Reflector.h Rotor.h RotorPositions.h
	g++ -g -Wall -c PlugBoard.cpp
Reflector.o: Reflector.cpp utils.h PlugBoard.h Reflector.h Rotor.h RotorPositions.h
	g++ -g -Wall -c Reflector.cpp
Rotor.o: Rotor.cpp utils.h PlugBoard.h Reflector.h Rotor.h RotorPositions.h
	g++ -g -Wall -c Rotor.cpp
RotorPositions.o: RotorPositions.cpp utils.h PlugBoard.h Reflector.h Rotor.h RotorPositions.h
	g++ -g -Wall -c RotorPositions.cpp

clean:
	rm -f *.o enigma
