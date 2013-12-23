#include "common/common.h"
#include "common/underived.h"

USING_NAMESPACE_COMMON

//#define ERROR_CASE

//Test_class类可以使用，但是不能被继承
class Test_class:public Underived<Test_class>
{
    public:
        Test_class() {}
        ~Test_class() {}
};

#ifdef ERROR_CASE
class Test_class1:public Test_class
{
    public:
        Test_class1() {}
        ~Test_class1() {}
};
#endif

class Test_class2:public Underived<Test_class2>
{
    public:
        Test_class2() {}
        ~Test_class2() {}
};

int main()
{
    {
        Test_class t;
        Test_class* p = new Test_class;
        delete p;

        Test_class2 t2;
        Test_class2* p2 = new Test_class2;
        delete p2;
#ifdef ERROR_CASE
        Test_class1 t1;
        Test_class1* p1 = new Test_class1;
        delete p1;
#endif
    }
    return 0;
};
