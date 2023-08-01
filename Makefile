all:
	g++ -std=c++11 -Wall -I include src/*.cpp src/visuals/*.cpp -o bin/p -F/Library/Frameworks -framework SDL2

clean:
	rm -rf bin