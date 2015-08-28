
CC = gcc
CFLAGS = -ggdb -W -Wall
LD = $(CC)
LDLIBS = -largtable2

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
DOBJ := $(SRC:.c=.d)

BINNAME = loftd

$(BINNAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDLIBS)

include $(OBJ:.o=.d)

%.d: %.c
	./depend.sh $(CFLAGS) $*.c > $@

# client.d: client.c
# 	./depend.sh `mysql_config --cflags` $(CFLAGS) client.c >client.d
# 
# client.o: client.c defs.h macros.h
# 	$(CC) `mysql_config --cflags` -o client.o -c client.c

clean:
	-rm -f $(DOBJ) $(OBJ) $(BINNAME)
