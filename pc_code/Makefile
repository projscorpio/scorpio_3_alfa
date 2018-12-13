CXX=g++
CXXFLAGS= -std=c++11 -Wall -Wextra -O3 -pedantic
INCLUDES=  include/*cc
SRC=control_rover.cc
OBJS=$(SRC:.c=.o)

TARGET=control_rover


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

.c.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	@rm  $(TARGET) $(MAIN)
