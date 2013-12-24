#ifndef __STRING_LIST__
#define __STRING_LIST__

#include "common.h"
#include "base_type.h"
#include <string>
#include <vector>

__COMMON_BEGIN_NAMESPACE

template<typename string_type=std::string, typename container_type=std::vector<string_type> >
class String_list
{
    public:
        void parse_string(string_type& _string, string_type& _split)
        {
            parse_string_base(_string, _split);
        }

        void parse_string(string_type& _string, char _split)
        {
            string_type split_str(1, _split);
            parse_string_base(_string, split_str);
        }

        void parse_string(const char* _str_p, size_t _str_size, const char* _split_p, size_t _split_size)
        {
            string_type string_(_str_p, _str_size);
            string_type split_(_split_p, _split_size);
            parse_string_base(string_, split_);
        }

        void parse_string(const char* _str_p, const char* _split_p)
        {
            string_type string_(_str_p);
            string_type split_(_split_p);
            parse_string_base(string_, split_);
        }

        void parse_string(const char* _str_p, char _split)
        {
            string_type string_(_str_p);
            string_type split_(1, _split);
            parse_string_base(string_, split_);
        }

        string_type operator[](Uint32 _index)
        {
            return (_index < m_container.size())?(m_container[_index]):(string_type());
        }

        Uint32 size()
        {
            return m_container.size();
        }

    private:
        void parse_string_base(string_type& _string, string_type& _split)
        {
            m_container.clear();

            if (!_string.length())
            {
                return;
            }

            if (!_split.length())
            {
                return;
            }

            if (_string.length() < _split.length())
            {
                return;
            }

            Uint32 head = 0;
            Uint32 tail = 0;

            while ((tail = _string.find_first_of(_split, head)) != string_type::npos)
            {
                if (tail > head)
                {
                    m_container.push_back(_string.substr(head, tail - head));
                }

                head = tail + 1;
            }

            if (head < _string.length() - 1)
            {
                m_container.push_back(_string.substr(head));
            }
        }

    private:
        container_type m_container;
};

__COMMON_END_NAMESPACE


#endif
