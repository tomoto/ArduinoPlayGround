#ifndef UtilityFunctions_h
#define UtilityFunctions_h

namespace tomoto {

#ifdef min
#undef min
#endif
template <typename T> T min(T a, T b) { return a < b ? a : b; }

#ifdef max
#undef max
#endif
template <typename T> T max(T a, T b) { return a > b ? a : b; }

}

#endif
