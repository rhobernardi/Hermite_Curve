#SOURCE = exercicio5-Rodrigo_Bernardi-8066395.cpp
SOURCE = Bezier_incompleto.cpp
#SOURCE = NURBS.cpp
BIN = ex5

all: compile run

compile:
	g++ $(SOURCE) -lglut -lGL -lGLEW -lGLU -o $(BIN)

zip: compile
	zip -r exercicio5-Rodrigo_Bernardi-8066395.zip $(SOURCE) $(BIN) Makefile

run:
	./$(BIN)
