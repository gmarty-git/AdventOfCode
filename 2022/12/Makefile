all : adventOfCode_2022_12.bin

adventOfCode_2022_12.bin : main.o class_Map.o
	g++ -o adventOfCode_2022_12.bin main.o class_Map.o -I./inc -O0 -g

main.o : ./src/main.cpp
	g++ -o main.o -c ./src/main.cpp -I./inc -O0 -g

class_Map.o : ./src/class_Map.cpp
	g++ -o class_Map.o -c ./src/class_Map.cpp -I./inc -std=c++11 -O0 -g

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_12.bin
