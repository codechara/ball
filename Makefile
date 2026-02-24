.PHONY: clean
all: ball

ball: main.cpp
	g++ main.cpp -o ball

clean:
	rm ball

