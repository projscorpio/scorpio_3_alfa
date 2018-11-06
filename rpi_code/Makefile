CXX=g++
CXXFLAGS= -Wall -pthread
INCLUDES= include/*.cc
SRC=main.cc
OBJS=$(SRC:.cc=.o)
LIBS= -lpigpio -lrt

all: prog.o


clean:
	@rm *.o *~ $(MAIN)

prog.o: main.cc 
	$(CXX) $(CXXFLAGS) -o prog $(SRC) $(INCLUDES) $(LIBS)
