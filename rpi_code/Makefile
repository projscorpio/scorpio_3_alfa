CXX=g++
CXXFLAGS= -Wall -pthread
INCLUDES= include/*cc
SRC=drive_rover.cc
OBJS=$(SRC:.cc=.o)
LIBS= -lpigpio -lrt

TARGET=drive_rover

all: $(TARGET)


clean:
	@rm *.o *~ $(MAIN)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(INCLUDES) $(LIBS)
