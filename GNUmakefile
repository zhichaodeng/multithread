
ifeq ($(DEBUG),yes)
  OPTIMIZE_FLAG = -O0 -g -DDEBUG
else
  OPTIMIZE_FLAG = -O3
endif

#XX=clang++
CXX = g++
LDFLAGS = -pthread 
CXXFLAGS = -std=c++11 -Wall -MMD -pedantic $(OPTIMIZE_FLAG)

SRC=$(wildcard *.cpp)
OBJ=$(SRC: .cpp=.o)
BIN=$(SRC: .cpp=)
DEPS = $(OBJ:.o=.d) 

all : $(BIN)

$(BIN): %.o
	$(CXX) $(LDFLAGS) $^ -o $@ 

$(OBJ): %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $^ -o $@ 

#%.d: %.cpp

# prevent a file is called clean
clean :
	rm -f *.o
#	rm $(BIN)

#-include $(DEPS)
