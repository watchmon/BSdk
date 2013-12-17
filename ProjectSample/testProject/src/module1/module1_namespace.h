#ifndef __MODULE1_NAMESPACE__
#define __MODULE1_NAMESPACE__

#if defined(__HAS_NAMESPACE) || !defined(__NO_NAMESPACE)
#	define MODULE1__ Module1__
#	define MODULE1_BEGIN_NAMESPACE namespace MODULE1__ {
#	define MODULE1_END_NAMESPACE }
#	define USING_NAMESPACE_MODULE1 using namespace MODULE1__;
#else
#	define MODULE1__
#	define MODULE1_BEGIN_NAMESPACE
#	define MODULE1_END_NAMESPACE
#	define USING_NAMESPACE_MODULE1
#endif

#endif
