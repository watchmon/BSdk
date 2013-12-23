#include "common/common.h"
#include "common/underived.h"

USING_NAMESPACE_COMMON

class Test_class:public Underived<Test_class>
{
    public:
        Test_class() {}
        ~Test_class() {}
};

class Test_class1:public Test_class
{
    public:
        Test_class1() {}
        ~Test_class1() {}
};

int main()
{
    {
        Test_class t;
        Test_class* p = new Test_class;
        delete p;
        Test_class1 t1;
        Test_class1* p1 = new Test_class1;
        delete p1;
    }
    return 0;
};
