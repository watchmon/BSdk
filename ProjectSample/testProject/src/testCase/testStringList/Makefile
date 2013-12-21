TARGET = test_string_list
LIBS = -L./../../../lib -lcommon
FORMAT = ../../../build/astyle -A1 -C -N -j -b -k1 -S -f -U -H -m0 -M80
CXXFLAGS = -I../../ -I. -w
CXX_DEBUG_FLAGS = -g -ggdb
CXX = g++

# 编译规则定义
#   1、依赖文件更新将重新编译(包括头文件)，由.d文件保证
# 	2、采用遍历目录方式，编译.c和.cpp文件，排除掉特定文件
#
# 注意一旦.d文件中依赖的文件不存在时会出错，需要手动删除.d文件才能正常编译。

.%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< > $@
	@$(CXX) $(CXXFLAGS) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | sed s/"\.o"/"\.d"/  >> $@

%.o: %.cpp
	$(CXX) $(CXX_DEBUG_FLAGS) $(CXXFLAGS) -c $<

.%.d: %.c
	$(CC) $(CCFLAGS) -MM $< > $@
	@$(CC) $(CCFLAGS) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | sed s/"\.o"/"\.d"/  >> $@

%.o: %.c
	$(CC) $(CC_DEBUG_FLAGS) $(CCFLAGS) -c $<

.%.d: %.cc
	$(CXX) $(CXXFLAGS) -MM $< > $@
	@$(CXX) $(CXXFLAGS) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | sed s/"\.o"/"\.d"/  >> $@

%.o: %.cc
	$(CXX) $(CXX_DEBUG_FLAGS) $(CXXFLAGS) -c $<

all : format $(TARGET)

format:
	$(FORMAT) *.cpp

test_string_list:test_string_list.o
	$(CXX) $^ $(LIBS) -o $@

clean:
	rm -rf *.o $(TARGET)
