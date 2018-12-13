CXX=g++
CXXFLAGS= -Wall -pthread -pedantic -O3 -Wextra
INCLUDES= include/*cc
SRC=drive_rover.cc
OBJS=$(SRC:.cc=.o)
LIBS= -lpigpio -lrt

TARGET=drive_rover

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(INCLUDES) $(LIBS)

clean:
	@rm $(TARGET)
