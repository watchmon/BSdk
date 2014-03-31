#ifndef _COMMON_INIFILE_H
#define _COMMON_INIFILE_H

//todo:为底层模块配置项，在全局添加
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "common.h"

__COMMON_BEGIN_NAMESPACE

using std::string;
using std::vector;
using std::map;
using std::less;

class Inifile
{
    public:
        typedef map< string, string, less<string> > strMap;
        typedef strMap::iterator strMapIt;

        Inifile();
        Inifile(const string& filename);

        ~Inifile();

        bool open(const string& pinipath);

        string read(const string& sect, const string& key);
        int get_value(const string& sect, const string& key, char* value, size_t size);
        int get_value(const string& sect, const string& key, int* value);
        int get_value(const string& sect, const string& key, unsigned int* value);
        int get_value(const string& sect, const string& key, long* value);
        int get_value(const string& sect, const string& key, unsigned long* value);
        int get_value(const string& sect, const string& key, double* value);
        int get_value(const string& sect, const string& key, string* value);

        // 返回所有section
        int get_section(vector<string>& sec);

        // 返回section下的所有key
        int get_key(const string& in_sec, vector<string>& key);

    protected:
        bool do_open(const string& inipath);

        strMap m_inimap;
};

struct analyzeini
{
    string m_strsect;
    Inifile::strMap* m_pmap;

    analyzeini(Inifile::strMap& strmap);

    void operator()(const string& in_strini);
};

__COMMON_END_NAMESPACE

#endif
