#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>

using std::vector;
using std::pair;
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::numeric_limits;

#include "FGR.h"

void usage(const char *exename)
{
  cout << "Usage: " << exename << " <benchname> <solnname> [options]" << endl;
  cout << "Available options:" << endl;
  cout << "  -o <filename>         Save using base <filename>" << endl;
  cout << endl;
}

int main(int argc, char **argv)
{
  if(argc < 3)
  {
    usage(argv[0]);
    return 0;
  }

  string outputName = "congestion";

  for(int i = 3; i < argc; ++i)
  {
    if(argv[i] == string("-h") || argv[i] == string("-help"))
    {
      usage(argv[0]);;
      return 0;
    }
    if(argv[i] == string("-o"))
    {
      if(i + 1 < argc)
      {
        outputName = argv[i+1];
      }
      else
      {
        cout << "option -o requires an argument" << endl;
        usage(argv[0]);
        return 0;
      }
    }
  }

  FGRParams parms;
  FGR fgr(parms);

  fgr.parseInputMapper(argv[1]);

  fgr.parseSolution(argv[2]);

  bool noCheckRouted = false;
  fgr.printStatistics(noCheckRouted);

  fgr.plotXPM(outputName);

  return 0;
}
