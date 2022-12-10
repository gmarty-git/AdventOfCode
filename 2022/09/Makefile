all : adventOfCode_2022_09.bin

adventOfCode_2022_09.bin : main.o class_Rope.o
	g++ -o adventOfCode_2022_09.bin main.o class_Rope.o -I./inc

main.o : ./src/main.cpp
	g++ -o main.o -c ./src/main.cpp -I./inc

class_Rope.o : ./src/class_Rope.cpp
	g++ -o class_Rope.o -c ./src/class_Rope.cpp -I./inc

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_09.bin
