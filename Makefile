OBJ = main.o
INC = -I "./"

Raytracer: $(OBJ)
	g++ $(OBJ) -o Raytracer
	rm -rf $(OBJ)

main.o:
	g++ -c main.cpp $(INC)

clean:
	rm -rf $(OBJ) Raytracer