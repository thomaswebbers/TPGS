.PHONY: clean

IDIR=include
ODIR=obj
SDIR=src

CC=gcc
CFLAGS=-I$(IDIR)

DEPS = $(wildcard $(IDIR)/*.h)
SRCS = $(wildcard $(SDIR)/*.c)
OBJ = $(patsubst $(SDIR)/%,$(ODIR)/%,$(SRCS:.c=.o))

$(ODIR)/%.o: $(SDIR)/%.c
	+@[ -d $(ODIR) ] || mkdir -p $(ODIR)
	$(CC) -MMD $(CFLAGS) -c -o $@ $<

tnes: $(OBJ) $(ODIR)/main.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	-rm -f $(ODIR)/*.o *~ core.* $(INCDIR)/*~
	-rm -f $(ODIR)/*.d
	-rm -f tnes
