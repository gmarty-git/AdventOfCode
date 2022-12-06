all : adventOfCode_2022_05.bin

adventOfCode_2022_05.bin : main.o class_Cargo.o
	g++ -o adventOfCode_2022_05.bin main.o class_Cargo.o

main.o : ./src/main.cpp
	g++ -o main.o -c ./src/main.cpp -I./inc

class_Cargo.o : ./src/class_Cargo.cpp
	g++ -o class_Cargo.o -c ./src/class_Cargo.cpp -I./inc

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_05.bin
