CPP = g++
COMPILE = -c -Wall
LINK = -Wall
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

image: $(OBJS)
	$(CPP) $(LINK) $^ -o $@
$(OBJS):%.o:%.cpp
	$(CPP) $(COMPILE) $< -o $@

.PHONY=clean cleanimg
clean:
	rm -rf *.o task image
cleanimg:
	rm -rf *.pgm *.ppm *.pbm