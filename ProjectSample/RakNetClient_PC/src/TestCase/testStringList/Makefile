TARGET = test_string_list
LIBS = -L./../../../lib -lcommon -lprofiler
FORMAT = ../../../build/astyle -A1 -C -N -j -b -k1 -S -f -U -H -m0 -M80
CXXFLAGS = -I../../ -I. -w
CXX_DEBUG_FLAGS = -g -ggdb
CXX = g++

all : format $(TARGET)

format:
	$(FORMAT) *.cpp

test_string_list:test_string_list.o
	$(CXX) $^ $(LIBS) -o $@
test_string_list.o:test_string_list.cpp
	$(CXX) $(CXX_DEBUG_FLAGS) $(CXXFLAGS) -c test_string_list.cpp

clean:
	rm -rf *.o $(TARGET)

remake:
	make clean
	make
