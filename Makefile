CXX = c++
CXXFLAGS = -Wall -std=c++17 -I/opt/homebrew/include -Iinclude
LDFLAGS = -L/opt/homebrew/lib -lglfw \
          -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

HEADERS = $(wildcard *.h include/*.h)

main: main.cpp helpers.cpp src/gl.c $(HEADERS)
	$(CXX) $(CXXFLAGS) -o main main.cpp helpers.cpp src/gl.c $(LDFLAGS)

clean:
	rm -f main