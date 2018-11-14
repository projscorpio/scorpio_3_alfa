CXX=g++
CFLAGS= -Wall -Wextra
INCLUDES= -I include/
SRC=control_rover.cc
OBJS=$(SRC:.c=.o)

TARGET=control_rover


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

.c.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	@rm *.o *~ $(MAIN)
