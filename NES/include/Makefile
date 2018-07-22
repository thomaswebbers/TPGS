.PHONY: clean

IDIR=include
ODIR=obj
SRCDIR=src

CC=gcc
CFLAGS=-I$(IDIR)

DEPS = $(wildcard $(IDIR)/*.h)
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%,$(ODIR)/%,$(SRCS:.c=.o))

$(ODIR)/%.o: $(SRCDIR)/%.c
	+@[ -d $(ODIR) ] || mkdir -p $(ODIR)
	$(CC) -MMD $(CFLAGS) -c -o $@ $<

hellomake: $(OBJ) $(ODIR)/main.o
	$(CC) -o $@ $^ $(CFLAGS) -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl

run_hellomake: hellomake
	./$<

clean:
	-rm -f $(ODIR)/*.o *~ core.* $(INCDIR)/*~
	-rm -f $(ODIR)/*.d
	-rm -f hellomake
