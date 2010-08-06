#if !defined(__CONFIGCACHE_H__) && !defined(NO_CONFIG_CACHE)
#define __CONFIGCACHE_H__

//#ifdef __GNUC__
//    #include <ext/hash_map>
//    namespace std { using namespace __gnu_cxx; }
//#else
//#ifdef _HAS_ITERATOR_DEBUGGING
//#undef _HAS_ITERATOR_DEBUGGING
//#endif
//    #define _HAS_ITERATOR_DEBUGGING 0
//    #include <hash_map>
//    namespace std { using namespace stdext; }
//#endif

#include "google/dense_hash_map"

#include "libconfig.h"
#include "stdTypes.h"

namespace libconfig
{
  typedef google::dense_hash_map<string, config_setting_t*> ConfigCache;
}

#endif //__CONFIGCACHE_H__