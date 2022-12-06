all : adventOfCode_2022_06.bin

adventOfCode_2022_06.bin : main.o utils_fileIO.o
	g++ -o adventOfCode_2022_06.bin main.o utils_fileIO.o

main.o : ./src/main.cpp
	g++ -o main.o -c ./src/main.cpp -I../utils/inc

utils_fileIO.o : ../utils/src/utils_fileIO.cpp
	g++ -o utils_fileIO.o -c ../utils/src/utils_fileIO.cpp -I../utils/inc

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_06.bin
