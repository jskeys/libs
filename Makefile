CC=gcc
IDIR=./include
SDIR=./src
ODIR=./build
CFLAGS=-I. -I$(IDIR) -Wall

DEP=$(wildcard $(IDIR)/*.h)
SRC=$(wildcard $(SDIR)/*.c)
OBJ = $(patsubst %.c, %.o, $(patsubst $(SDIR)/%, $(ODIR)/%, $(SRC)))

# The item to the left is what you're trying to make. The items to the
# right are the dependencies required to make that. If a dependency does
# not exist, the makefile will look for a rule to make it.
all: $(OBJ)

$(ODIR)/%.o: $(SDIR)/%.c $(DEP) $(ODIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(ODIR):
	$(shell mkdir -p $(ODIR))

format:
	@find ./src ./include -regex ".*\.[ch]p*" | xargs clang-format -i -style=file --verbose

doc: $(SRC)
	$(shell doxygen)

clean:
	rm -rf $(ODIR) ./doc/html ./doc/latex

.PHONY: all format
