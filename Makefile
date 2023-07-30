all:
	g++ -Wall -Iinclude src/*.cpp -o p -F/Library/Frameworks -framework SDL2

clean:
	rm p