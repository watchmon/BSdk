#ifndef __STRING_LIST_BASE__
#define __STRING_LIST_BASE__

#include "common.h"
#include "base_type.h"
#include <vector>

__COMMON_BEGIN_NAMESPACE

template<typename ST=std::string, typename CT = std::vector<ST> >
class String_list_base
{
	public:
		typedef ST string_type;
		typedef CT container_type;

	public:
		String_list_base(){}

		string_type operator[](Uint32 _index)
		{
			return m_container[_index];
		}

		Uint32 size()
		{
			return m_container.size();
		}
		
	protected:
		void parse_string_base(string_type &_string, string_type &_split)
		{
			m_container.clear();

			if (!_string.length())
				return;

			if (!_split.length())
				return;

			if (_string.length() < _split.length())
				return;
			
			Uint32 head = 0;
			Uint32 tail = 0;

			while ( (tail = _string.find_first_of(_split, head) ) != string_type::npos )
			{
				if (tail > head)
					m_container.push_back(_string.substr(head, tail - head));
				head = tail + 1;
			}
			
			if ( head < _string.length() - 1)
				m_container.push_back(_string.substr(head));
		}


	private:
		container_type m_container;
		string_type    m_split;
};

__COMMON_END_NAMESPACE

#endif
