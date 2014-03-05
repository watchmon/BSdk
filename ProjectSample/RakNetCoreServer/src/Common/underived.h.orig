#ifndef __UNDERIVED__
#define __UNDERIVED__

#include "common.h"

__COMMON_BEGIN_NAMESPACE

template<typename T>
class Underived_base
{
	public:
		struct Underived_base_tool
		{
			typedef T Type;
		};

		friend class Underived_base_tool::Type;

	protected:
		Underived_base(){}

		~Underived_base(){}
};

template<typename T>
class Underived:virtual private Underived_base<T>
{
	public:
		struct Underived_tool
		{
			typedef T Type;
		};

	protected:
		Underived(){}

		~Underived(){}
};

__COMMON_END_NAMESPACE


#endif
