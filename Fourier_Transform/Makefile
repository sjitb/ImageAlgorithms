all: main.cpp image.o ipTool.o ROI.o SOI.o
	g++ -g *.o main.cpp -o ipTools -lmpeg -lrfftw -lfftw -lm

image.o:
	g++ -g -c image.cpp

ipTool.o:
	g++ -g -c ipTool.cpp

ROI.o:
	g++ -g -c ROI.cpp

SOI.o:
	g++ -g -c SOI.cpp

clean:
	rm ipTools -f
	rm *.o -f
