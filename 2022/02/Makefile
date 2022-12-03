all : adventOfCode_2022_02.bin

adventOfCode_2022_02.bin : main.o utils_fileIO.o utils_string.o
	g++ -o adventOfCode_2022_02.bin main.o utils_fileIO.o utils_string.o

main.o : ./src/main.cpp
	g++ -o main.o -c ./src/main.cpp -I../utils/inc

utils_fileIO.o : ../utils/src/utils_fileIO.cpp
	g++ -o utils_fileIO.o -c ../utils/src/utils_fileIO.cpp -I../utils/inc

utils_string.o : ../utils/src/utils_string.cpp
	g++ -o utils_string.o -c ../utils/src/utils_string.cpp -I../utils/inc

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_02.bin
