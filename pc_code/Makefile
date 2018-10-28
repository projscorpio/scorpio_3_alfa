CC=g++
CFLAGS= -Wall -Wextra
INCLUDES= -I./include/*cc
SRC=controll_rover.cc
OBJS=$(SRC:.c=.o)
LIBS= -lWiring

TARGET=control_rover


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	@rm *.o *~ $(MAIN)
