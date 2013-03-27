CC=g++
SRCDIR=src
OBJDIR=obj

all: jarvis

_OBJ = main.o Commands.o Compute.o Devices.o Parser.o State.o
OBJ = $(patsubst %, $(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $<

jarvis: $(OBJ)
	$(CC) $^ -o $@

