all : adventOfCode_2022_11.bin

adventOfCode_2022_11.bin : main.o class_MonkeyGang.o
	g++ -o adventOfCode_2022_11.bin main.o class_MonkeyGang.o -I./inc

main.o : ./src/main.cpp
	g++ -o main.o -c ./src/main.cpp -I./inc

class_MonkeyGang.o : ./src/class_MonkeyGang.cpp
	g++ -o class_MonkeyGang.o -c ./src/class_MonkeyGang.cpp -I./inc -std=c++17

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_11.bin
