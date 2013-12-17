include ../../build/rules.mk

TARGET = libcommon.a
SRC_SKIP = 
SRCS = $(filter-out $(SRC_SKIP), $(wildcard *.c) $(wildcard *.cpp))
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SRCS)))
DEPS = $(patsubst %.o, .%.d, $(OBJS))

all : format $(TARGET)

format:
	$(FORMAT) *.h *.cpp

$(TARGET) : $(OBJS)
	$(AR) -r $@ $+
	cp $@ ../../lib

-include $(DEPS)

clean:
	rm -rf *.o .*.d $(TARGET)
