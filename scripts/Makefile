all: realclean main 

main: TBGenerator.o StEncoder.o StructGenerator.o main.o
	g++ -o main StEncoder.o StructGenerator.o TBGenerator.o  main.o

StEncoder.o: StEncoder.cpp StEncoder.h
	g++ -c StEncoder.cpp -std=c++11

StructGenerator.o: StructGenerator.cpp StructGenerator.h
	g++ -c StructGenerator.cpp -std=c++11

TBGenerator.o: TBGenerator.cpp TBGenerator.h
	g++ -c TBGenerator.cpp -std=c++11

main.o: main.cpp StEncoder.h StructGenerator.h 
	g++ -c main.cpp -std=c++11

clean:
	rm -rf main StEncoder.o StructGenerator.o TBGenerator.o main.o
	
realclean: clean
	rm -rf ../src/*   ../tb/*
