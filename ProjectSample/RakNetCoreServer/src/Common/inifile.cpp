#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "inifile.h"

using namespace std;

__COMMON_BEGIN_NAMESPACE

static const char* const MIDDLESTRING = "_____***_______";

analyzeini::analyzeini(Inifile::strMap& strmap):m_pmap(&strmap)
{
}

void analyzeini::operator()(const string& in_strini)
{
    string strini = in_strini;
    string::size_type first = 0;
    string::size_type last = 0;

    // 去除#号注释
    while (1)
    {
        if ((first = strini.find_first_of("#",first)) == string::npos)
        {
            break;
        }

        // 将\#转为#
        if (strini[first-1] == '\\')
        {
            // 删除'\'
            strini.erase(first-1,1);

            // 从#号之后开始再匹配注释
            first++;
            continue;
        }

        strini.erase(first);
        break;
    }

    // 解释[]
    first = strini.find('[');
    last = strini.rfind(']');

    if ((first != string::npos) &&
        (last != string::npos) &&
        (first != last+1))
    {
        // 去除[]中的前后空格
        first = strini.find_first_not_of(" \t",first+1);
        last = strini.find_last_not_of(" \t",last-1);

        if ((first != string::npos) &&
            (last != string::npos) &&
            (first != last+1))
        {
            m_strsect = strini.substr(first, last-first+1);
        }

        return ;
    }

    if (m_strsect.empty())
    {
        return;
    }

    string strkey; // key
    string strvalue; // value

    // 解释key=value
    if ((first=strini.find('=')) == string::npos)
    {
        strkey = strini;
        // 截去key前后的空字符
        first = strkey.find_first_not_of(" \t");
        last = strkey.find_last_not_of(" \t");

        if ((first == string::npos) || (last == string::npos))
        {
            return ;
        }

        strkey = strkey.substr(first, last-first+1);
    }
    else
    {
        strkey = strini.substr(0,first);
        strvalue = strini.substr(first+1, string::npos);

        // 截去key前后的空字符
        first = strkey.find_first_not_of(" \t");
        last = strkey.find_last_not_of(" \t");

        if ((first == string::npos) || (last == string::npos))
        {
            return ;
        }

        strkey = strkey.substr(first, last-first+1);

        // 截去value前后的空字符
        first = strvalue.find_first_not_of(" \t");
        last = strvalue.find_last_not_of(" \t");

        if ((first == string::npos) || (last == string::npos))
        {
            return ;
        }

        strvalue = strvalue.substr(first, last-first+1);
    }

    // section_____***_______key 组合
    (*m_pmap)[m_strsect + MIDDLESTRING + strkey] = strvalue;
    //cout<<"key= "<<m_strsect + MIDDLESTRING + strkey<<" value= "<<strvalue<<endl;
    return ;
}

Inifile::Inifile()
{
}

Inifile::Inifile(const string& filename)
{
    do_open(filename);
}

Inifile::~Inifile()
{
}

bool Inifile::open(const string& inipath)
{
    return do_open(inipath);
}

string Inifile::read(const string& sect, const string& key)
{
    strMapIt it = m_inimap.find(sect + MIDDLESTRING + key);

    if (it == m_inimap.end())
    {
        return "";
    }
    else
    {
        return it->second;
    }
}

int Inifile::get_value(const string& sect, const string& key, char* value, size_t size)
{
    memset(value,0,size);
    read(sect, key).copy(value,size);
    return 0;
}

int Inifile::get_value(const string& sect, const string& key, int* value)
{
    *value = atoi(read(sect, key).c_str());
    return 0;
}

int Inifile::get_value(const string& sect, const string& key, unsigned int* value)
{
    *value = strtoul(read(sect, key).c_str(), NULL, 10);
    return 0;
}

int Inifile::get_value(const string& sect, const string& key, long* value)
{
    *value = atol(read(sect, key).c_str());
    return 0;
}

int Inifile::get_value(const string& sect, const string& key, unsigned long* value)
{
    *value = strtoul(read(sect, key).c_str(), NULL, 10);
    return 0;
}

int Inifile::get_value(const string& sect, const string& key, double* value)
{
    *value = atof(read(sect, key).c_str());
    return 0;
}

int Inifile::get_value(const string& sect, const string& key, string* value)
{
    *value = read(sect, key);
    return 0;
}

int Inifile::get_section(vector<string>& sec)
{
    string tmp1,tmp2;

    for (strMapIt it=m_inimap.begin(); it != m_inimap.end(); ++it)
    {
        tmp2 = it->first.substr(0, it->first.find(MIDDLESTRING));

        if (tmp1 != tmp2)
        {
            sec.push_back(tmp2);
            tmp1=tmp2;
        }
    }

    return 0;
}

int Inifile::get_key(const string& in_sec, vector<string>& key)
{
    string sec_mid = in_sec+MIDDLESTRING;

    for (strMapIt it=m_inimap.begin(); it!=m_inimap.end(); ++it)
    {
        if (it->first.compare(0,sec_mid.length(),sec_mid) == 0)
        {
            key.push_back(it->first.substr(sec_mid.length()));
        }
    }

    return 0;
}

bool Inifile::do_open(const string& inipath)
{
    if (0 != access(inipath.c_str(), R_OK))
    {
        return false;
    }

    ifstream fin(inipath.c_str());

    if (!fin.is_open())
    {
        return false;
    }

    vector<string> strvect;

    while (!fin.eof())
    {
        string inbuf;
        getline(fin, inbuf,'\n');
        strvect.push_back(inbuf);
    }

    if (strvect.empty())
    {
        return false;
    }

    for_each(strvect.begin(), strvect.end(), analyzeini(m_inimap));

    return !m_inimap.empty();
}

__COMMON_END_NAMESPACE
