CC=g++
SRCDIR=src
OBJDIR=obj

all: jarvis jarvist

#_OBJ = main.o Commands.o Compute.o Devices.o Parser.o State.o
_OBJ = Commands.o Compute.o Devices.o Parser.o State.o
OBJ = $(patsubst %, $(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(SRCDIR)/%.h
#$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $<

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

jarvis: $(OBJ) $(OBJDIR)/main.o
	$(CC) $^ -o $@

jarvist: $(SRCDIR)/test.cpp jarvis
	$(CC) $< -o $@

clean:
	rm $(OBJDIR)/*.o jarvis jarvist

