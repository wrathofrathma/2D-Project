CC=g++ -std=c++17
CFLAGS=-Wall -I. -O2 -s -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLEW -pthread -lGLU -pthread -g
OBJDIR=obj
SRCDIR = ./src
SRC:=main.cpp $(shell find $(SRCDIR) -name "*.cpp")
OBJ:=$(SRC:%.cpp=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) -c -o $@  $< $(CFLAGS)

prog: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(shell find $(OBJDIR) -name "*.o")

run:
	./prog
