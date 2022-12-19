all : adventOfCode_2022_14.bin day14_anim.bin

day14_anim.bin : day14_anim.cpp
	g++ -o day14_anim.bin day14_anim.cpp -lSDL2

adventOfCode_2022_14.bin : main.o 
	g++ -o adventOfCode_2022_14.bin main.o

main.o : main.cpp
	g++ -o main.o -c main.cpp

clean:
	rm -rf *.o

mrproper: clean
	rm -rf adventOfCode_2022_14.bin
