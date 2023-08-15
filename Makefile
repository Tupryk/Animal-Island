all:
	g++ -std=c++11 -Wall -I include src/*.cpp src/visuals/*.cpp src/beings/*.cpp src/terrain/*.cpp -o bin/p -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_gfx

clean:
	rm -rf bin