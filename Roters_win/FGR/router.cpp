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

int main(int argc, char **argv)
{
  cout << "FGR " << FGRversion << " (" << sizeof(void*)*8 << "-bit) Compiled on "<<"???"; /// << COMPILETIME;
#ifdef __GNUC__
  cout << " with GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
#endif
  cout << endl;

  // parameters
  string outputfile;

  for(int i = 2; i < argc; ++i)
  {
    if(argv[i] == string("-o"))
    {
      if(i+1 < argc)
      {
        outputfile = argv[++i];
      }
      else
      {
        cout << "option -o requires an argument" << endl;
        FGRParams::usage(argv[0]);
        return 0;
      }
    }
  }

  FGRParams parms(argc, argv);

  FGR fgr(parms);

  // read in the nets and create the grid
  fgr.parseInput(argv[1]);

  cout << "CPU time: " << cpuTime() << " seconds" << endl;

  fgr.printParams();

  fgr.initialRouting();

  double rrrStartCpu = cpuTime();

  fgr.doRRR();

  cout << endl << "after all rip-up iterations" << endl;
  fgr.printStatistics();

  double rrrEndCpu = cpuTime();

  cout << "RRR CPU time: " << rrrEndCpu-rrrStartCpu << " seconds " << endl;

  cout << endl << "assigning layers" << endl;
  fgr.layerAssignment();

  cout << "after layer assignment" << endl;
  fgr.printStatistics();

  cout << endl << "greedy improvement phase" << endl;
  double greedyStartCpu = cpuTime();
  fgr.greedyImprovement();

  cout << endl << "after greedy improvement phase" << endl;
  fgr.printStatistics();

  double greedyEndCpu = cpuTime();

  cout << "greedy CPU time: " << greedyEndCpu-greedyStartCpu << " seconds " << endl;

  cout << endl;
  fgr.printStatistics(true,true);

  if(!parms.outputFile.empty())
  {
    fgr.writeRoutes(parms.outputFile);
  }

  system("PAUSE");
  return 0;
}
