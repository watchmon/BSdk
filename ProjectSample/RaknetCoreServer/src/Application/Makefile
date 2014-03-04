include ../../build/rules.mk

TARGET = raknetCoreServer
LIBS = -L../../lib -lpthread -lNetworkModule -lCommon -lRaknet
SRC_SKIP = 
SRCS = $(filter-out $(SRC_SKIP), $(wildcard *.c) $(wildcard *.cpp))
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SRCS)))
DEPS = $(patsubst %.o, .%.d, $(OBJS))
#CXXFLAGS += `mysql_config --include` `pkg-config --cflags protobuf-lite`

all : format $(TARGET)

format:
	$(FORMAT) *.cpp

$(TARGET) : $(OBJS) ../../lib/libCommon.a ../../lib/libNetworkModule.a ../../lib/libRaknet.a
	@for file in $(DEPS); do if [ -e $$file ] && [ "$$(cat $$file)" == "" ]; then rm $$file; fi; done
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@
	cp $(TARGET) ../../bin

-include $(DEPS)

clean:
	rm -rf *.o .*.d $(TARGET)
