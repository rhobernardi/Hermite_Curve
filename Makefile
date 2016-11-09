SOURCE = HermiteCurve.cpp
BIN = HermiteCurve

all: compile run

compile:
	g++ $(SOURCE) -lglut -lGL -lGLEW -lGLU -o $(BIN)

run:
	./$(BIN)
