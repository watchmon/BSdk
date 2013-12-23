#ifndef __STRING_LIST__
#define __STRING_LIST__

#include "string_list_base.h"

__COMMON_BEGIN_NAMESPACE

class String_list:public String_list_base<>
{
	public:
		typedef String_list_base<>::string_type string_type;
		typedef String_list_base<>::container_type container_type;

	public:
		String_list();
		~String_list();

		void parse_string(string_type &_string, string_type &_split);

		void parse_string(string_type &_string, char _split);

		void parse_string(const char *_str_p, size_t _str_size, const char *_split_p, size_t _split_size);

		void parse_string(const char *_str_p, const char *_split_p);

		void parse_string(const char *_str_p, char _split);
};

__COMMON_END_NAMESPACE


#endif
