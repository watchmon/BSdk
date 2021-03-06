#ifndef __NETWORK_MODULE_NAMESPACE__
#define __NETWORK_MODULE_NAMESPACE__

#if defined(__HAS_NAMESPACE) || !defined(__NO_NAMESPACE)
#	define NETWORK_MODULE__ Network_module__
#	define NETWORK_MODULE_BEGIN_NAMESPACE namespace NETWORK_MODULE__ {
#	define NETWORK_MODULE_END_NAMESPACE }
#	define USING_NAMESPACE_NETWORK_MODULE using namespace NETWORK_MODULE__;
#else
#	define NETWORK_MODULE__
#	define NETWORK_MODULE_BEGIN_NAMESPACE
#	define NETWORK_MODULE_END_NAMESPACE
#	define USING_NAMESPACE_NETWORK_MODULE
#endif

#endif
