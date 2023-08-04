OBJ = main.o
INC = -I "./"

Raytracer: $(OBJ)
	g++ $(OBJ) -o Raytracer -fopenmp
	rm -rf $(OBJ)

main.o:
	g++ -c main.cpp $(INC) -fopenmp

clean:
	rm -rf $(OBJ) Raytracer