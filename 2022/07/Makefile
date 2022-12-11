all : adventOfCode_2022_07.bin

adventOfCode_2022_07.bin : main.o class_ElvesFilesytem.o
	g++ -o adventOfCode_2022_07.bin main.o class_ElvesFilesytem.o

main.o : ./src/main.cpp
	g++ -o main.o -c ./src/main.cpp -I./inc

class_ElvesFilesytem.o : ./src/class_ElvesFilesytem.cpp
	g++ -o class_ElvesFilesytem.o -c ./src/class_ElvesFilesytem.cpp -I./inc

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_07.bin
