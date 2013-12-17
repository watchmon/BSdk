#ifndef __MODULE2_NAMESPACE__
#define __MODULE2_NAMESPACE__

#if defined(__HAS_NAMESPACE) || !defined(__NO_NAMESPACE)
#	define MODULE2__ Module2__
#	define MODULE2_BEGIN_NAMESPACE namespace MODULE2__ {
#	define MODULE2_END_NAMESPACE }
#	define USING_NAMESPACE_MODULE2 using namespace MODULE2__;
#else
#	define MODULE2__
#	define MODULE2_BEGIN_NAMESPACE
#	define MODULE2_END_NAMESPACE
#	define USING_NAMESPACE_MODULE2
#endif

#endif
