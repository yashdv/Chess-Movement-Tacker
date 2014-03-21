all:
	g++ -g -ggdb `pkg-config --cflags opencv` -c morphOpen.cpp -o morphOpen.o
	g++ -g -ggdb `pkg-config --cflags opencv` -c mean.cpp -o mean.o
	g++ -g -ggdb `pkg-config --cflags opencv` -c position.cpp -o position.o
	g++ -g -ggdb `pkg-config --cflags opencv` -c hough.cpp -o hough.o
	g++ -g -ggdb `pkg-config --cflags opencv` -c chessmove.cpp -o chessmove.o
	g++ -g -ggdb `pkg-config --cflags opencv` -c updatemove.cpp -o updatemove.o
	g++ -g -o "chess" -ggdb `pkg-config --cflags opencv` position.o mean.o morphOpen.o hough.o chessmove.o updatemove.o main.cpp `pkg-config --libs opencv`
	g++ -g -o "board" -ggdb `pkg-config --cflags opencv` board.cpp `pkg-config --libs opencv`

clean:
	rm *.o
	rm moves/*
