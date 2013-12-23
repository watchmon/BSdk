TARGET = test_underived
LIBS = -L./../../../lib -lcommon
FORMAT = ../../../build/astyle -A1 -C -N -j -b -k1 -S -f -U -H -m0 -M80
CXXFLAGS = -I../../ -I. -w
CXX_DEBUG_FLAGS = -g -ggdb
CXX = g++


all : format $(TARGET)

format:
	$(FORMAT) *.cpp

test_underived:test_underived.o
	$(CXX) -o $(TARGET) test_underived.o
test_underived.o:test_underived.cpp
	$(CXX) $(CXXFLAGS) -c test_underived.cpp

clean:
	rm -rf *.o $(TARGET)
