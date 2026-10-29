CC          := g++
CFLAGS      := -Wall -Werror -O2
OBJDIR      := obj
SRCDIR      := src
SRC         := $(wildcard $(SRCDIR)/*.cpp)
OBJ         := $(SRC:.cpp=.o)
BIN         := exe

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $^ -o $@ `pkg-config --cflags --libs sdl3`
	mkdir $(OBJDIR)
	mv $(OBJ) $(OBJDIR)

%.o: %.cpp clean
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN) $(OBJDIR)