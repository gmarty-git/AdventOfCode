all : adventOfCode_2022_04.bin

adventOfCode_2022_04.bin : main.o
	g++ -o adventOfCode_2022_04.bin main.o

main.o : ./src/main.cpp
	g++ -o main.o -c ./src/main.cpp

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_04.bin
