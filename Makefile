# A simple Makefile written by Justin.Zhao

TARGET := playcoding
OBJS := playcoding.o
SRCS := ball.c data.c windows.c playcoding.c
BIN := ~/bin/

# complier
CC := gcc
# linker
LD := ld
# archiver
AR := ar

# command
DUMP := objdump
RM := rm -rf
ECHO := echo
MV := mv

#INCLUDES := -I/usr/include
INCLUDES := $(shell pkg-config --cflags gtk+-3.0)
#LIBS := -L/xxx/xxx/lib
LIBS := $(shell pkg-config --libs gtk+-3.0)
#DFLAGS := -DMY_MACRO=1 
DFLAGS :=
CFLAGS := -c -g -W -Wall -O2
LFLAGS := 

$(TARGET) :
	$(CC) $(INCLUDES) -o $@ $(SRCS) $(LIBS)

temp : $(OBJS)
	$(LD) $(LFLAGS) -o $@ $< $(LIBS)
#	$(DUMP) -h -S $< > $(TARGET).elf

%.o : %.c
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS) *.elf

install:
	$(MV) $(TARGET) $(BIN)
    
# claim the pseduo target
.PHONY: clean install
