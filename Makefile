CC=g++
SRCDIR=src
OBJDIR=obj

all: jarvis

#_OBJ = main.o Commands.o Compute.o Devices.o Parser.o State.o
_OBJ = Commands.o Compute.o Devices.o Parser.o State.o
OBJ = $(patsubst %, $(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(SRCDIR)/%.h
#$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $<

main.o: $(SRCDIR)/main.cpp
	$(CC) -c $(SRCDIR)/main.cpp -o main.o

jarvis: $(OBJ) main.o
	$(CC) $^ -o $@

clean:
	rm $(OBJDIR)/*.o jarvis

