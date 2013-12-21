#include <iostream>
#include "common/common.h"
#include "common/string_list.h"

using namespace std;
USING_NAMESPACE_COMMON

int main()
{
    String_list list;
    string test_str1("fdasjkf:jfdkslajfd;ls:fjdj:1231:::::fjds:::");
    string test_split(":");
    list.parse_string(test_str1, test_split);
    cout << "原始字符串" << endl;
    cout << test_str1 << endl;
    cout << "分割后的字符串" << endl;

    for (int i = 0; i < list.size(); i++)
    {
        cout << list[i] << endl;
    }

    cout << endl;
    cout << endl;

    list.parse_string("123fjkdlsa:jfjfjf:21382910::::::", ":");

    for (int i = 0; i < list.size(); i++)
    {
        cout << list[i] << endl;
    }

    cout << endl;
    cout << endl;

    list.parse_string("123fjkdlsa:jfjfjf:21382910::::::", ':');

    for (int i = 0; i < list.size(); i++)
    {
        cout << list[i] << endl;
    }

    return 0;
}

