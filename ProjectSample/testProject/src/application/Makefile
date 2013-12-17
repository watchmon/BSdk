include ../../build/rules.mk

TARGET = testApplication
LIBS = -L../../lib -lpthread -lmodule1 -lmodule2 -lcommon
SRC_SKIP = 
SRCS = $(filter-out $(SRC_SKIP), $(wildcard *.c) $(wildcard *.cpp))
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SRCS)))
DEPS = $(patsubst %.o, .%.d, $(OBJS))
#CXXFLAGS += `mysql_config --include` `pkg-config --cflags protobuf-lite`

all : format $(TARGET)

format:
	$(FORMAT) *.cpp

$(TARGET) : $(OBJS) ../../lib/libcommon.a ../../lib/libmodule1.a ../../lib/libmodule2.a
	@for file in $(DEPS); do if [ -e $$file ] && [ "$$(cat $$file)" == "" ]; then rm $$file; fi; done
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@
	cp $(TARGET) ../../bin

-include $(DEPS)

clean:
	rm -rf *.o .*.d $(TARGET)
