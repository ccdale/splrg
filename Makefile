
CC = gcc
CFLAGS = -ggdb -W -Wall
LD = $(CC)
LDLIBS = -largtable2

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
DOBJ := $(SRC:.c=.d)

BINNAME = splrgd

$(BINNAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDLIBS)

include $(OBJ:.o=.d)

%.d: %.c
	./depend.sh $(CFLAGS) $*.c > $@

clean:
	-rm -f $(DOBJ) $(OBJ) $(BINNAME)
