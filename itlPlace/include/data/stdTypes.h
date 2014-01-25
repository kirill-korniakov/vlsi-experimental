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

typedef std::vector<int> IntVector;
typedef std::vector<double> DblVector;
typedef std::vector<int>::iterator IntIterator;
typedef std::vector<int>::const_iterator IntCIterator;
typedef std::vector<double>::iterator DblIterator;
typedef std::vector<double>::const_iterator DblCIterator;

#ifdef NOMINMAX

template<typename T> 
inline T min(T a, T b) {return a < b ? a : b; }

template<typename T> 
inline T max(T a, T b) {return a < b ? b : a; }

#endif

inline int Round(double val) { return int(val > 0.0 ? val + 0.5 : val - 0.5); }

inline bool Equals(double v1, double v2, const double eps = 1e-8)
{
  if (v1 >= 0.0)
  {
    if (v2 <= 0.0)
    {
      return v1 - v2 < eps;
    }
    else
    {
      return v1 < v2 ? (v2 - v1 < v2 * eps) : (v1 - v2 < v1 * eps);
    }
  }
  else if (v1 < 0.0)
  {
    if (v2 >= 0.0)
    {
      return v2 - v1 < eps;
    }
    else
    {
      return v1 < v2 ? (v1 - v2 > v1 * eps) : (v2 - v1 > v2 * eps);
    }
  }
  return false;
}

template<class T>
inline bool Equals(T v1, T v2)
{
  return v1 == v2;
}

inline double ManhattanLength(double x1, double y1, double x2, double y2)
{
  return (x1 > x2 ? x1 - x2 : x2 - x1) + (y1 > y2 ? y1 - y2 : y2 - y1);
}


#endif //__STDTYPES_H__