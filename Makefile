CC=	g++
CFLAGS=	-Iinclude
PROG=   raytracer
SRCS=   src/FileParser.cpp src/Primitives.cpp src/Raytracer.cpp src/main.cpp
OBJS=	$(SRCS:.cpp=.o)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) 


%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

Raytracer.o: include/Raytracer.h include/Primitives.h include/Operations.h vector.h stdio.h stdlib.h iostream
Primitives.o: include/Primitives.h math.h 
FileParser.o: include/FileParser.h include/Primitives.h vector iostream fstream sstream 
main.o: include/Primitives.h include/FileParser.h include/Raytracer.h vector string

clean:
	rm -f *~ $(OBJS) $(PROG)
