#include "string_list.h"

__COMMON_BEGIN_NAMESPACE

String_list::String_list()
{
}
String_list::~String_list()
{
}

void String_list::parse_string(string_type &_string, string_type &_split)
{
	parse_string_base(_string, _split);
}

void String_list::parse_string(string_type &_string, char _split)
{
	string_type split_str(1, _split);
	parse_string_base(_string, split_str);
}

void String_list::parse_string(const char *_str_p, size_t _str_size, const char *_split_p, size_t _split_size)
{
	string_type string_(_str_p, _str_size);
	string_type split_(_split_p, _split_size);
	parse_string_base(string_, split_);
}

void String_list::parse_string(const char *_str_p, const char *_split_p)
{
	string_type string_(_str_p);
	string_type split_(_split_p);
	parse_string_base(string_, split_);
}

void String_list::parse_string(const char *_str_p, char _split)
{
	string_type string_(_str_p);
	string_type split_(1, _split);
	parse_string_base(string_, split_);
}

__COMMON_END_NAMESPACE
