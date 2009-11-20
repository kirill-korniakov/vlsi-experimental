#ifndef __STDTYPES_H__
#define __STDTYPES_H__

#include <string>
#include <list>
#include <exception>
#include <stack>
#include <queue>
#include <vector>

typedef int IDType;

typedef std::string string;
typedef std::exception exception;

template <class T>
class TemplateTypes
{
public:
  typedef std::list<T> list;
  typedef std::stack<T> stack;
  typedef std::queue<T> queue;
  typedef std::vector<T> vector;
};

#ifdef NOMINMAX

template<typename T> 
inline T min(T a, T b) {return a < b ? a : b; }

template<typename T> 
inline T max(T a, T b) {return a < b ? b : a; }

#endif

inline int Round(double val) { return int(val > 0.0 ? val + 0.5 : val - 0.5); }


#endif //__STDTYPES_H__