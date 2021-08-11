CC=gcc
BDIR=./bin
IDIR=./include
ODIR=./obj
SDIR=./src
TDIR=./test
CFLAGS=-I. -I$(IDIR) -Wall -Wpedantic -Wextra

DEP=$(wildcard $(IDIR)/*.h)
SRC=$(wildcard $(SDIR)/*.c)

OBJ+=$(patsubst %.c, %.o, $(patsubst $(SDIR)/%, $(ODIR)/%, $(SRC)))
OBJ+=$(patsubst %.c, %.o, $(patsubst $(TDIR)/%, $(ODIR)/%, $(TST)))


# The item to the left is what you're trying to make. The items to the
# right are the dependencies required to make that. If a dependency does
# not exist, the makefile will look for a rule to make it.
all: $(OBJ)

# Compile object files from source contained in src/
$(ODIR)/%.o: $(SDIR)/%.c $(DEP) $(ODIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(ODIR):
	$(shell mkdir -p $(ODIR))

$(BDIR):
	$(shell mkdir -p $(BDIR))

$(TDIR):
	$(shell mkdir -p $(TDIR))

format:
	@find ./src ./include -regex ".*\.[ch]p*" | xargs clang-format -i -style=file --verbose

doc: $(SRC)
	@doxygen

clean:
	rm -rf $(ODIR) ./doc/html ./doc/latex

.PHONY: all doc format
