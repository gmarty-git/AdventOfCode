all : adventOfCode_2022_13.bin

adventOfCode_2022_13.bin : main.o 
	g++ -o adventOfCode_2022_13.bin main.o 

main.o : main.cpp
	g++ -o main.o -c main.cpp

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_13.bin
