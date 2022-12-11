all : adventOfCode_2022_10.bin

adventOfCode_2022_10.bin : main.o
	g++ -o adventOfCode_2022_10.bin main.o -I./inc

main.o : ./src/main.cpp
	g++ -o main.o -c ./src/main.cpp -I./inc

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_10.bin
