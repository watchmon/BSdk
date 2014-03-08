#ifndef _COMMON_H
#define _COMMON_H

# if defined(__HAS_NAMESPACES) || !defined(__NO_NAMESPACES)
#   define __COMMON Common
#   define __COMMON_BEGIN_NAMESPACE namespace __COMMON {
#   define __COMMON_END_NAMESPACE }
#   define USING_NAMESPACE_COMMON using namespace __COMMON;
# else
#   define __COMMON
#   define __COMMON_BEGIN_NAMESPACE
#   define __COMMON_END_NAMESPACE
#   define USING_NAMESPACE_COMMON
# endif

#endif
